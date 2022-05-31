#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 

const float SINCOSMAX = 1;
const float SINCOSZERO = 0;
const float SINCOSMIN = -1;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x3F, 16, 2);

unsigned long a = 0;
unsigned long count = 0;
unsigned int lastDisplay = 0;

float b = 0;
bool show = false;
bool modeSinCos = false;
bool modeTgCtg = false;

enum state
{
  Cosinus,
  Sinus,
  Tangens,
  Cotangens,
  None
};

state Stan = None;

void setup()
{
    Serial.begin(9600);
    lcd.begin();
    lcd.backlight();
    lcd.clear();
}
void loop()
{
        char customKey = customKeypad.getKey();
        if (customKey=='1'||customKey=='2'||customKey=='3'||customKey=='4'||customKey=='5'||customKey=='6'||customKey=='7'||customKey=='8'||customKey=='9'||customKey=='0')
        {
           a = a * 10 + (customKey-'0');
           Display();
        }
        else if(customKey=='*')
        {
           if(show)
              show = false;
           else if(a!=0)
           {
              a = a / 10;
           }
           else
           {
               Stan = None;
               show = false;
           }
           Display();
        }
        else if(customKey=='#')
        {
          if(Stan==Sinus)
          {
               b = sinus(a);
               show = true;
          }
          else if(Stan == Cosinus)
          {
               b = cosinus(a);
               show = true;
          }
          else if(Stan == Tangens)
          {
            if(a%180!=90)
            {
              b = tangens(a);
              show = true;
            }
            else
            {
              show = false;
            }
          }
          else if(Stan == Cotangens)
          {
            if(a%180!=0)
            {
              b = cotangens(a);
              show = true;
            }
            else
            {
              show = false;
            }
            
          }
          Display();
        }
        else if(customKey=='A')
        {
          modeSinCos = !modeSinCos;
          if(modeSinCos)
          {
             Stan = Sinus;            
          }
          else
          {
            Stan = Cosinus;
          }
          a = 0;
          show = false;
          Display();
        }
        else if(customKey=='B')
        {
          modeTgCtg = !modeTgCtg;
          if(modeTgCtg)
          {
            Stan = Tangens;
          }
          else
          {
            Stan = Cotangens;
          }
          a = 0;
          show = false;
          Display();
        }
        else if(customKey=='C')
        {
          Display();
        }
        else if(customKey=='D')
        {
          Display();
        }
}
void Display()
{
  lcd.clear();
  lcd.setCursor(0,0);
  if(Stan==Sinus)
  {
    lcd.print("sin(");
    lcd.print(a);
    lcd.print(")");
  }
  else if(Stan == Cosinus)
  {
    lcd.print("cos(");
    lcd.print(a);
    lcd.print(")");
  }
  else if(Stan == Tangens)
  {
    lcd.print("tan(");
    lcd.print(a);
    lcd.print(")");
  }
  else if(Stan == Cotangens)
  {
    lcd.print("ctg(");
    lcd.print(a);
    lcd.print(")");
  }
  if(show)
  {
    lcd.print("=");
    if(Length(a)>5)
    {
     lcd.setCursor(0,1);
    }
    if(b==SINCOSMAX||b==SINCOSMIN)
      {
         lcd.print(b);
      }
      else if(b>1)
      {
          float fractPart = FractPart(b);
          int f = b - fractPart;
          lcd.print(f);
          lcd.print(".");
          if(fractPart<0.1)
          {
            lcd.print("0");
          }
          int fract = fractPart*1000;
          lcd.print(fract);
      }
      else if(b<-1)
      {
          lcd.print("-");
          float fractPart = FractPart(b);
          int f = b + fractPart;
          lcd.print(abs(f));
          lcd.print(".");
          int fract = fractPart*1000;
          lcd.print(abs(fract));
      }
      else
      {
        if(b<0)
        {
          lcd.print("-0.");
          if(b>-0.1)
            lcd.print("0");
          int x = abs(b) * 1000;
          lcd.print(x);
        }
        else
        {
            lcd.print("0.");
            if(b<0.1)
              lcd.print("0");
            int x = b * 1000;
            lcd.print(x);
        }
      }
    }
}
float sinus(unsigned long degree)
{
    if(degree%180==90)
    {
      return (degree/180)%2==1 ? -1: 1;
    }
    else if(degree%180==0)
    {
      return 0;
    }
    if(degree/180%2==0)
    { 
        degree = degree % 180;
        float x = convertToRad(degree);
        return (x-(pow(x,3)/6)+(pow(x,5)/120)-(pow(x,7)/5040)+(pow(x,9)/362880)-(pow(x,11)/39916800)+(pow(x,13)/6227020800)-(pow(x,15)/1307674368000)+(pow(x,17)/355687428096000)-(pow(x,19)/121645100408832000)+(pow(x,21)/51090942171709440000)-(pow(x,23)/25852016738884976640000));
    }
    else
    {
        degree = degree % 180;
        float x = convertToRad(degree);
        return -(x-(pow(x,3)/6)+(pow(x,5)/120)-(pow(x,7)/5040)+(pow(x,9)/362880)-(pow(x,11)/39916800)+(pow(x,13)/6227020800)-(pow(x,15)/1307674368000)+(pow(x,17)/355687428096000)-(pow(x,19)/121645100408832000)+(pow(x,21)/51090942171709440000)-(pow(x,23)/25852016738884976640000));
    }
}
float cosinus(unsigned long degree)
{
    if(degree%180==90)
    {
      return 0;
    }
    else if(degree%180==0)
    {
      return (degree/180)%2==0 ? 1: -1;
    }
    degree = degree % 360;
    if(degree>180)
    {
       float x = convertToRad(degree-180);
       x = 1-(pow(x,2)/2)+(pow(x,4)/24)-(pow(x,6)/720)+(pow(x,8)/40320)-(pow(x,10)/3628800)+(pow(x,12)/479001600)-(pow(x,14)/87178291200)+(pow(x,16)/20922789888000)-(pow(x,18)/6402373705728000)+(pow(x,20)/2432902008176640000)-(pow(x,22)/1124000727777607680000);
       return -x;
    }
    else
    {
       float x = convertToRad(degree);
       x = 1-(pow(x,2)/2)+(pow(x,4)/24)-(pow(x,6)/720)+(pow(x,8)/40320)-(pow(x,10)/3628800)+(pow(x,12)/479001600)-(pow(x,14)/87178291200)+(pow(x,16)/20922789888000)-(pow(x,18)/6402373705728000)+(pow(x,20)/2432902008176640000)-(pow(x,22)/1124000727777607680000);
       return x;
    }
}
float tangens(unsigned long degree)
{
  float x = sinus(degree)/cosinus(degree);
  x = abs(x);
  return degree%180<90 ? x: -x;
}
float cotangens(unsigned long degree)
{
  float x = cosinus(degree)/sinus(degree);
  x = abs(x);
  return degree%180<90 ? x: -x;
}
float convertToRad(unsigned long x)
{
    return x * 3.14159265359 / 180;
}
int Length(unsigned long x)
{
  int l = 0;
  do 
  {
     l++;
     x = x/10;
  }
  while(x!=0);
  return l;
}
float FractPart(float b)
{
  return b-(int)b;
}
