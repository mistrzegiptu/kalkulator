#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte degreeChar[8]
{
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x3F, 16, 2);

unsigned long a = 0;
unsigned long count = 0;
unsigned int lastDisplay = 0;

float result = 0;

bool show = false;
bool point = false;
bool minus = false;

int trigState=0;
int arcusState=0;

String fractional = "";

enum state
{
  Cosine,
  Sine,
  Tangent,
  Cotangent,
  Asine,
  Acosine,
  Atangent,
  Acotangent,
  None
};

state Stan = None;

void setup()
{
    Serial.begin(9600);
    lcd.begin();
    lcd.backlight();
    lcd.clear();
    lcd.createChar(0,degreeChar);
}
void loop()
{
        char customKey = customKeypad.getKey();
        if (customKey=='1'||customKey=='2'||customKey=='3'||customKey=='4'||customKey=='5'||customKey=='6'||customKey=='7'||customKey=='8'||customKey=='9'||customKey=='0')
        {
            if(show)
            {
              show = false;
            }
            if(!point)
                a = a * 10 + (customKey-'0');
            else
            {
              if(fractional.length() < 3)
                  fractional.concat(customKey);
            }
            Display();
        }
        else if(customKey=='*')
        {
           if(show)
              show = false;
           else if(fractional!="")
           {
              fractional.remove(fractional.length()-1);
           }
           else if(point)
           {
                point = false;
                fractional = "";
           }
           else if(a!=0)
           {
              a = a / 10;
           }
           else
           {
               minus = false;
               Stan = None;
               show = false;
               arcusState = 0;
               trigState = 0;
           }
           Display();
        }
        else if(customKey=='A')
        {
          trigState++;
          if(trigState>4)
          {
            trigState = 1;
          }
          switch(trigState)
          {
            case 1:
              Stan = Sine;
              break;
            case 2:
              Stan = Cosine;
              break;
            case 3:
              Stan = Tangent;
              break;
            case 4:
              Stan = Cotangent;
              break;
          }
          a = 0;
          fractional = "";
          arcusState = 0;
          point = false;
          show = false;
          minus = false;
          Display(); 
        }
        else if(customKey=='B')
        {
          arcusState++;
          if(arcusState>4)
          {
            arcusState = 1;
          }
          switch(arcusState)
          {
            case 1:
              Stan = Asine;
              break;
            case 2:
              Stan = Acosine;
              break;
            case 3:
              Stan = Atangent;
              break;
            case 4:
              Stan = Acotangent;
              break;
          }
          a = 0;
          fractional = "";
          trigState = 0;
          point = false;
          show = false;
          minus = false;
          Display();
        }
        else if(customKey=='C')
        {
            minus = !minus;
            Display();
        }
        else if(customKey=='D')
        {
          if(Stan==Sine)
          {
               result = sine(a, StringToFloat(fractional));
               if(minus)
                  result = -result;
               show = true;
          }
          else if(Stan == Cosine)
          {
               result = cosine(a, StringToFloat(fractional));
               show = true;
          }
          else if(Stan == Tangent)
          {
            if(a%180!=90||a%180==90&&StringToFloat(fractional)!=0)
            {
              result = tangent(a, StringToFloat(fractional));
              if(minus)
                  result = -result;
              show = true;
            }
            else
            {
              show = false;
            }
          }
          else if(Stan == Cotangent)
          {
            if(a%180!=0||a%180==0&&StringToFloat(fractional)!=0)
            {
              result = cotangent(a, StringToFloat(fractional));
              if(minus)
                  result = -result;
              show = true;
            }
            else
            {
              show = false;
            }
            
          }
          else if(Stan == Asine)
          {
            if(a==0||a==1&&StringToFloat(fractional)==0)
            {
              result = RadToDeg(asine(a+StringToFloat(fractional)));
              if(minus)
                  result = -result;
              show = true;
            }
            else
            {
              show = false;
            }
          }
          else if(Stan==Acosine)
          {
            if(a==0||a==1&&StringToFloat(fractional)==0)
            {
              if(minus)
              {
                  result = 180-RadToDeg(acosine(a+StringToFloat(fractional)));
              }
              else
              {
                result = RadToDeg(acosine(a+StringToFloat(fractional)));
              }
              show = true;
            }
            else
            {
              show = false;
            }
          }
          else if(Stan == Atangent)
          {
              result = RadToDeg(atangent(a+StringToFloat(fractional)));
              if(minus)
                  result = -result;
              show = true;
          }
          else if(Stan==Acotangent)
          {
            result = RadToDeg(acotangent(a+StringToFloat(fractional)));
            if(minus)
            {
                result = RadToDeg(PInum-acotangent(a+StringToFloat(fractional)));
            }
            show = true;
          }
          Display();
        }
        else if(customKey=='#')
        {
          point = true;
          Display();
        }
}
void Display()
{
  String displaying = "";
  lcd.clear();
  lcd.setCursor(0,0);
  switch(Stan)
  {
    case Sine:
      displaying.concat("sin(");
      break;
    case Cosine:
      displaying.concat("cos(");
      break;
    case Tangent:
      displaying.concat("tan(");
      break;
    case Cotangent:
      displaying.concat("cot(");
      break;
    case Asine:
      displaying.concat("asin(");
      break;
    case Acosine:
      displaying.concat("acos(");
      break;
    case Atangent:
      displaying.concat("atan(");
      break;
    case Acotangent:
      displaying.concat("acot(");
      break;
  }
  if(Stan!=None)
  {
      if(minus)
        displaying.concat("-");
      displaying.concat(a);
      if(point)
        displaying.concat(".");
        displaying.concat(fractional);
      if(show)
      {
        displaying.concat(")=");
        displaying.concat(result);
      }
      for(int i = 0; i < displaying.length(); i++)
      {
        if(i==16)
        {
          lcd.setCursor(0,1);
        }
        lcd.print(displaying[i]);
      }
      if(show&&(Stan==Asine||Stan==Acosine||Stan==Atangent||Stan==Acotangent))
      {
         lcd.write((byte)0);
      }
  }
}
///// TAYLOR SERIES FOR TRIG FUNCIONS
float sine(unsigned long degree, float f)
{
    if(degree%180==90&&f==0)
    {
      return (degree/180)%2==1 ? -1: 1;
    }
    else if(degree%180==0&&f==0)
    {
      return 0;
    }
    if(degree/180%2==0)
    { 
        degree = degree % 180;
        float x = convertToRad(degree+f);
        return (x-(pow(x,3)/6)+(pow(x,5)/120)-(pow(x,7)/5040)+(pow(x,9)/362880)-(pow(x,11)/39916800)+(pow(x,13)/6227020800)-(pow(x,15)/1307674368000)+(pow(x,17)/355687428096000)-(pow(x,19)/121645100408832000)+(pow(x,21)/51090942171709440000)-(pow(x,23)/25852016738884976640000));
    }
    else
    {
        degree = degree % 180;
        float x = convertToRad(degree+f);
        return -(x-(pow(x,3)/6)+(pow(x,5)/120)-(pow(x,7)/5040)+(pow(x,9)/362880)-(pow(x,11)/39916800)+(pow(x,13)/6227020800)-(pow(x,15)/1307674368000)+(pow(x,17)/355687428096000)-(pow(x,19)/121645100408832000)+(pow(x,21)/51090942171709440000)-(pow(x,23)/25852016738884976640000));
    }
}
float cosine(unsigned long degree, float f)
{
    if(degree%180==90&&f==0)
    {
      return 0;
    }
    else if(degree%180==0&&f==0)
    {
      return (degree/180)%2==0 ? 1: -1;
    }
    else if(degree%180==60&&f==0)
    {
      return (degree/180)%2==0 ? 0.5: -0.5;
    }
    degree = degree % 360;
    if(degree>180)
    {
       float x = convertToRad(degree-180+f);
       x = 1-(pow(x,2)/2)+(pow(x,4)/24)-(pow(x,6)/720)+(pow(x,8)/40320)-(pow(x,10)/3628800)+(pow(x,12)/479001600)-(pow(x,14)/87178291200)+(pow(x,16)/20922789888000)-(pow(x,18)/6402373705728000)+(pow(x,20)/2432902008176640000)-(pow(x,22)/1124000727777607680000);
       return -x;
    }
    else
    {
       float x = convertToRad(degree+f);
       x = 1-(pow(x,2)/2)+(pow(x,4)/24)-(pow(x,6)/720)+(pow(x,8)/40320)-(pow(x,10)/3628800)+(pow(x,12)/479001600)-(pow(x,14)/87178291200)+(pow(x,16)/20922789888000)-(pow(x,18)/6402373705728000)+(pow(x,20)/2432902008176640000)-(pow(x,22)/1124000727777607680000);
       return x;
    }
}
float tangent(unsigned long degree, float f)
{
  float x = sine(degree,f)/cosine(degree,f);
  x = abs(x);
  return degree%180<90 ? x: -x;
}
float cotangent(unsigned long degree, float f)
{
  float x = cosine(degree,f)/sine(degree,f);
  x = abs(x);
  return degree%180<90 ? x: -x;
}
///// HORNER'S FORMULAS FOR ARCUS FUNCTION (HORNER>>>TAYLOR)
float asine(float value)
{
    if(value==0.5)
        return 0.523598776;
    return atangent(value/sqrt(1-pow(value,2)));
}
float acosine(float value)
{
    return PInum/2-asine(value);
}
float atangent(float value)
{
    float x = value, xx;
    float a0=1.0,
    a1 = 0.33288950512027,
    a2 = -0.08467922817644,
    a3 = 0.03252232640125,
    a4 = -0.00749305860992;
    if(x>=1)
    {
        xx = pow(1/x,2);
        return PInum/2-(1/x)/poly(xx,a0,a1,a2,a3,a4);
    }
    else if(x<=-1)
    {
        xx = pow(1/x,2);
        return -(PInum/2-(1/x))/poly(xx,a0,a1,a2,a3,a4);
    }
    else
    {
        xx = pow(x,2);
        return x / poly(xx,a0,a1,a2,a3,a4);
    }
}
float acotangent(float value)
{
    return atangent(1/value);
}
float poly(float x, float a0, float a1, float a2, float a3, float a4)
{
    return a0+(a1+(a2+(a3+a4*x)*x)*x)*x;
}
//// USEFULL FUNCIONS FOR DISPLAYING NUMBERS ON THE SCREEN
float convertToRad(float x)
{
  return x * 3.14159265359 / 180;
}
float RadToDeg(float rad)
{
   return rad * 180 / PInum;
}
float StringToFloat(String s)
{
  if(s==""||s.toInt()==0)
    return 0;
  else
    return (float)s.toInt()/pow(10,s.length());
}
