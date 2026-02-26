#include <ncurses.h>
#include <stdbool.h>

int ch = 0;

void zoom() {
    int top = 0, left = 0, bot = LINES-1, right = COLS-1;
    int ch;

    while (1) {
        // draw corners of current quadrant
        mvaddch(top, left, (chtype)219);
        mvaddch(top, right, (chtype)219);
        mvaddch(bot, left, (chtype)219);
        mvaddch(bot, right, (chtype)219);
        move(top, left);
        refresh();

        ch = getch();

        int midY = (top + bot) / 2;
        int midX = (left + right) / 2;

        switch(ch) {
            case 'e': bot = midY; right = midX; break; // top-left
            case 'y': bot = midY; left = midX;  break; // top-right
            case 'c': top = midY; right = midX; break; // bot-left
            case 'b': top = midY; left = midX;  break; // bot-right
            default: return; // exit zoom on anything else
        }
    }
}

int main()
{
    bool normalMode = false;
    //what is usage of this?

    //initialization statements
    initscr();
    //this line is very importnat it alows the escape sequences (spit out by mouse position to be read)
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    MEVENT event;
    printf("\033[?1003h\n"); // Enable all mouse motions
    raw();
    keypad(stdscr, TRUE);
    noecho(); 

    while(ch != 'q') {
        
        if (normalMode == false && ch >= 32 && ch <= 126)
        {
            //in every function that writes output jus thave a mirror function that takes some arguments to decide what is being written and update the text file accordingly.
            printw("%c", ch);
        }
        //grab mouse posiition
        //this statement pulls out the printing of any mouse related characters. we only wnat ch to be printed if it originated from the keyboard 
        else if(normalMode == false && ch == KEY_MOUSE) {
            if(getmouse(&event) == OK) {
                //move there
                move(event.y, event.x);
                mvprintw(0, 0, "Mouse is at Row: %d, Col: %d   ", event.y, event.x);
                move(event.y, event.x);
            }
        }
        else if(ch == 27)
        {
            normalMode = !normalMode;
        }
        else if(normalMode == true)
        {
            if (ch == 91)
                zoom();
        }
        refresh();
        ch = getch();
    }

    printf("\033[?1003l\n");
    endwin();
    //mouse hover and position is returend
    //typewriter effect cursor dosent move
    //as much as i want it to be like vim with the hjkl it just isnt effeceint for a 2d space thats why mouse is better or maybe a more nuanced thing. like define 5 keys 4 representing corners and one representing center and lets say you press top left corner then you will zoom into that corner and you will press again to be further in that corner. That type of thing and you exit out of that navigation and maybe another set of navigation based on querying words and for that it would use resovoir rather than scraping everything on screen. I think having the text file being written to and edited at runtime and manipulated is the best and the things being printed are coming from the text file. Also another mode based on just manuevering between the entries like for this it would just jump from locations of actual values to values.
        // because think abotu ti fundamentally vim was a ehypereffecienzation of the medium of typing but i am trying to combine two mediums so i must have something unique. whats special about paper is you can literally move your hand anywhere on it and really hte most effecietn adn fluid way to emulate that is with the mouse. just try it out mimic like you were using the program with you rmouse. maybe also an element of like capturing that text and moving it with the cursor. like you can maybe keep typing adn then lets say you want to bring it closer to something else you just press control and it picks up what you wrote nad brings it closer. Maybe this can be at runtime not using text resovoir beause the final location should be the only one like affecting other shits because like what if you overshoot when pressing control but you dont let go but it still replaces something also its a bad use of runtime effeciency to constantly update.
            // maybe even more effecticve if you just capture the hover position while ctrl is pressed and display a charavter at that spot so it signifies something being dragged and whereever you let go that is the location is pulls and updates text resovoir of data with the location of th thing being. Then that is read and displayed. you can think of the dispaly printing as having two layers one layer of which being the display of the text resovoir as it is read and then a layer on top displaying stuff like the aforementioned character symbol for dragging or whatever.
    // for the nuanced command system lets say user passes command of 1dw then you split that inpout into 3 things the integer 1 and the character d and character w. the first one represents how many times to do it so it would be set as the upper value in a for loop within which would be a switch corresponding to the character and that has some interaction with w as well. maybe another character before number that specifies type of command so that its evne more flexible and there could be some command like do previous action again but ig that would just fit into this one so its fine.
    //esc as toggle between modes
    return 0;
}
