#include <UTFT.h>

// Declare which fonts we will be using
extern uint8_t BigFont[];

// Set the pins to the correct ones for your development shield
UTFT myGLCD(ITDB50,38,39,40,41);

class Color
{
public:
    unsigned char r, g, b;
    Color(unsigned char r, unsigned char g, unsigned char b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

class Number
{
    int start_x;
    int start_y;
    int segment_width;
    int segment_height;
    Color* foreground;
    Color* background;
    int nr;
    int last_displayed_nr;
    int space;
    static const int nr_columns = 3;
    static const int nr_rows = 5;
    static const int nr_segments = 15;
    static const int font_description[10][15];

public:
    Number(int x, int y, int segment_width, int segment_height, int space, int nr, Color* foreground, Color* background)
    {
        this->start_x = x;
        this->start_y = y;
        this->segment_width = segment_width;
        this->segment_height = segment_height;
        this->nr = nr;
        this->foreground = foreground;
        this->background = background;
        this->space = space;
        last_displayed_nr = -1;
    }

    int GetWidth()
    {
        return nr_columns * segment_width + nr_columns * space;
    }

    int GetHeight()
    {
        return nr_rows * segment_height + nr_rows * space;
    }

    void SetNumber(int nr)
    {
        if (nr >= 0 && nr <= 9)
        {
            this->nr = nr;
        }
    }

    int GetNr()
    {
        return nr;
    }

    void SetX(int new_val)
    {
        start_x = new_val;
    }

    void SetY(int new_val)
    {
        start_y = new_val;
    }

    void Clear()
    {
        myGLCD.setColor(background->r, background->g, background->b);
        myGLCD.fillRect(start_x, start_y, start_x + GetWidth(), start_y + GetHeight());
    }

    void Draw()
    {
        if (last_displayed_nr == nr)
        {
            return;
        }
        int x = 0;
        int y = 0;
        int width = segment_width;
        int height = segment_height;
        int column = 0;
        int row = 0;

        for (int i = 0; i < nr_segments; i++)
        {
            x = start_x + column * width + column * space;
            y = start_y + row * height + row * space;

            if (last_displayed_nr != -1)
            {
                if (font_description[last_displayed_nr][i] == 0 && font_description[nr][i] == 1)
                {
                    myGLCD.setColor(foreground->r, foreground->g, foreground->b);
                    myGLCD.fillRect(x, y, x + width, y + height);
                }
                else if (font_description[last_displayed_nr][i] == 1 && font_description[nr][i] == 0)
                {
                    myGLCD.setColor( background->r, background->g, background->b);
                    myGLCD.fillRect(x, y, x + width, y + height);
                }
            }
            else
            {
                if (font_description[nr][i])
                {
                    myGLCD.setColor(foreground->r, foreground->g, foreground->b);
                    myGLCD.fillRect(x, y, x + width, y + height);
                }
            }
            column = (i + 1) % nr_columns;
            row += (column == 0) ? 1 : 0;
        }
        last_displayed_nr = nr;
    }
};

const int Number::font_description[10][15] = \
{
    { 1, 1, 1,   1, 0, 1,   1, 0, 1,   1, 0, 1,   1, 1, 1 },  // 0
    { 0, 1, 0,   1, 1, 0,   0, 1, 0,   0, 1, 0,   1, 1, 1 },  // 1
    { 1, 1, 1,   0, 0, 1,   1, 1, 1,   1, 0, 0,   1, 1, 1 },  // 2
    { 1, 1, 1,   0, 0, 1,   1, 1, 1,   0, 0, 1,   1, 1, 1 },  // 3
    { 1, 0, 1,   1, 0, 1,   1, 1, 1,   0, 0, 1,   0, 0, 1 },  // 4
    { 1, 1, 1,   1, 0, 0,   1, 1, 1,   0, 0, 1,   1, 1, 1 },  // 5
    { 1, 1, 1,   1, 0, 0,   1, 1, 1,   1, 0, 1,   1, 1, 1 },  // 6
    { 1, 1, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1 },  // 7
    { 1, 1, 1,   1, 0, 1,   1, 1, 1,   1, 0, 1,   1, 1, 1 },  // 8
    { 1, 1, 1,   1, 0, 1,   1, 1, 1,   0, 0, 1,   1, 1, 1 }   // 9
};

class ClockDisplay
{
    Number* h1;
    Number* h2;
    Number* m1;
    Number* m2;
    Number* s1;
    Number* s2;
    int space;
    int dot_x;
    int dot_y;
    int x,y, width, height;
    Color* fg;
    Color* bg;

public:
    ClockDisplay(Color* fg, Color* bg, int x, int y, int width = 30, int height = 18, int space = 5 )
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->fg = fg;
        this->bg = bg;
        this->space = space;

        h1 = new Number(x, y, width, height, space, 0, fg, bg);
        int digit_width = h1->GetWidth();

        x += digit_width + 2 * space;
        h2 = new Number(x, y, width, height, space, 0, fg, bg);

        x += digit_width + 2 * space;
        dot_x = x;
        dot_y = h2->GetHeight() / 3;

        x += 6 * space;
        m1 = new Number(x, y, width, height, space, 0, fg, bg);
        x += digit_width + 2 * space;
        m2 = new Number(x, y, width, height, space, 0, fg, bg);

        x += digit_width + 5 * space;
        s1 = new Number(x, y, width / 2, height / 2, space, 0, fg, bg);
        digit_width = s1->GetWidth();
        int digit_delta = h1->GetHeight() - s1->GetHeight();
        x += digit_width + 2 * space;
        y = y + digit_delta;
        s1->SetY(y);
        s2 = new Number(x, y, width / 2, height / 2, space, 0, fg, bg);
    }

    void SetTime(int h1, int h2, int m1, int m2, int s1, int s2)
    {
        this->h1->SetNumber(h1);
        this->h2->SetNumber(h2);

        this->m1->SetNumber(m1);
        this->m2->SetNumber(m2);

        this->s1->SetNumber(s1);
        this->s2->SetNumber(s2);
    }

    ~ClockDisplay()
    {
        delete h1;
        delete h2;
        delete m1;
        delete m2;
        delete s1;
        delete s2;
    }

    void DrawClock()
    {
        h1->Draw();
        h2->Draw();

        if (h1->GetNr() != -1)
        {
            myGLCD.setColor(fg->r, fg->g, fg->b);
            myGLCD.fillRect( dot_x, y + dot_y, dot_x + width / 2, y + dot_y + height / 2 );

            myGLCD.fillRect( dot_x, y + dot_y * 2, dot_x + width / 2, y + dot_y * 2 + height / 2);
        }

        m1->Draw();
        m2->Draw();

        s1->Draw();
        s2->Draw();
    }
};

class Clock
{
    int hours;
    int minutes;
    int seconds;
    ClockDisplay* cd;
    Color* fg;
    Color* bg;

public:
    Clock(int h, int m, int s = 0)
    {
        hours = h;
        minutes = m;
        seconds = s;
        fg = new Color(255, 255, 255);
        bg = new Color(0, 0, 0);
        cd = new ClockDisplay(fg, bg, 20, 20);
    }

    void Tick()
    {
        seconds++;
        if (seconds >= 60)
        {
            seconds = 0;
            minutes++;
        }

        if (minutes >= 60)
        {
            minutes = 0;
            hours++;
        }

        if (hours >= 24)
        {
            hours = 0;
        }

        int h1, h2, m1, m2, s1, s2;
        GetDigits(&h1, &h2, &m1, &m2, &s1, &s2);
        cd->SetTime(h1, h2, m1, m2, s1, s2);
    }

    void GetDigits(int* h1, int* h2, int* m1, int* m2, int* s1, int* s2)
    {
        *h1 = hours / 10;
        *h2 = hours - *h1 * 10;

        *m1 = minutes / 10;
        *m2 = minutes - *m1 * 10;

        *s1 = seconds / 10;
        *s2 = seconds - *s1 * 10;
    }

    void Display()
    {
        cd->DrawClock();
    }

};

//ClockDisplay cd(&fg, &bg, 20, 20);
int n = 0;
Clock c(20, 15, 0);

void setup()
{
    //randomSeed(analogRead(0));

    myGLCD.InitLCD();
    myGLCD.setFont(BigFont);
    myGLCD.clrScr();

    myGLCD.setColor(0, 255, 0);
    myGLCD.setBackColor(0, 0, 255);
    myGLCD.print("Runtime: (msecs)", CENTER, 450);
    myGLCD.printNumI(millis(), CENTER, 465);

    //cd.SetTime( n, n, n, n, n, n);

}

void loop()
{
    c.Tick();
    c.Display();
 //   cd.DrawClock();
//    n++;
//    n = (n > 9) ? 0 : n;
//    cd.SetTime( n, n, n, n, n, n);
    // Clear the screen and draw the frame
    delay (994);
}

