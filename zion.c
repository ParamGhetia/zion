#include <ncurses.h>
#include <stdbool.h>

int main()
{
    int ch = 0; 
    bool normalMode = false;

    //initialization statements
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho(); 

    while(ch != 'q') {
        if (ch == 27) {
            normalMode = true;
        }

        if (normalMode) {
            int y, x;
            getyx(stdscr, y, x);
            switch (ch) {
                case 'k': move(y-1, x); break;
                case 'j': move(y+1, x); break;
                case 'h': move(y, x-1); break;
                case 'l': move(y, x+1); break;
                case 'i': normalMode = false; break;
            }
        } else {
            // insert mode — print the character
            printw("%c", ch);
        }

        refresh();
        ch = getch();
    }

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
