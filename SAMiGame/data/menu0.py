import MenuBuilder

# main menu info
mainMenuSize = (500, 372)
mainMenuColor = (220, 220, 200)
mainMenuDirection = 3

menuTextSize = (400, 200)
menuTextColor = (0, 0, 0)
menuTextFontSize = 48

mainMenuButtonSize = (200, 40)

subMenuSize = (mainMenuSize[0]-16, mainMenuSize[1]-menuTextSize[1]-mainMenuButtonSize[1]-32)
subMenuColor = (215, 140, 165)
subMenuDirection = 1

subMenuButtonSize = ( (subMenuSize[0]-24)//3, 60 )


buttonColor = (255, 200, 220)
buttonFontSize = 24



# world stuff
world1Color = (80, 160, 220)
world1ButtonColor = (80, 120, 180)
world1SubmenuColor = (80, 100, 180)

world2Color = (180, 40, 40)
world2ButtonColor = (100, 100, 100)
world2SubmenuColor = (100, 40, 30)






menus = (
    # main menu
    MenuBuilder.Menu(*mainMenuSize, *mainMenuColor, 0, mainMenuDirection,
        (
        MenuBuilder.Text(*menuTextSize, *menuTextColor, menuTextFontSize, 'Main Menu'),
        ),
        (
        MenuBuilder.Button(*mainMenuButtonSize, *buttonColor, buttonFontSize, 'Play', 0, 1),
        ),
        (),
    ),

    
    # World 1 menu
    MenuBuilder.Menu(*mainMenuSize, *world1Color, 1, mainMenuDirection,
        (
            MenuBuilder.Text(*menuTextSize, *menuTextColor, menuTextFontSize, 'World 1'),
        ),
        (
            MenuBuilder.Button(*mainMenuButtonSize, *world1ButtonColor, buttonFontSize, 'Play', 0, 10),
        ),
        (
            MenuBuilder.Menu(*subMenuSize, *world1SubmenuColor, 0, 0,
                (),
                (
                    MenuBuilder.Button(*subMenuButtonSize, *world1ButtonColor, buttonFontSize, 'Next', 0, 2, 1),
                    MenuBuilder.Button(*subMenuButtonSize, *world1ButtonColor, buttonFontSize, 'Back', 0, 0),
                ),
                ()
            ),
        ),
    ),
    # World 2 menu
    MenuBuilder.Menu(*mainMenuSize, *world2Color, 2, mainMenuDirection,
        (
            MenuBuilder.Text(*menuTextSize, *menuTextColor, menuTextFontSize, 'World 2'),
        ),
        (
            MenuBuilder.Button(*mainMenuButtonSize, *world2ButtonColor, buttonFontSize, 'Play', 0, 20),
        ),
        (
            MenuBuilder.Menu(*subMenuSize, *world2SubmenuColor, 0, subMenuDirection,
                (),
                (
                    MenuBuilder.Button(*subMenuButtonSize, *world2ButtonColor, buttonFontSize, 'Prev', 0, 1),
                    MenuBuilder.Button(*subMenuButtonSize, *world2ButtonColor, buttonFontSize, 'Back', 0, 0),
                    MenuBuilder.Button(*subMenuButtonSize, *world2ButtonColor, buttonFontSize, 'Next', 0, 3, 2),
                ),
                ()
            ),
        ),
    ),

    
    # Level 1-1 menu
    MenuBuilder.Menu(*mainMenuSize, *world1Color, 10, mainMenuDirection,
        (
            MenuBuilder.Text(*menuTextSize, *menuTextColor, menuTextFontSize, 'Level 1'),
        ),
        (
            MenuBuilder.Button(*mainMenuButtonSize, *world1ButtonColor, buttonFontSize, 'Play', 1, 1),
        ),
        (
            MenuBuilder.Menu(*subMenuSize, *world1SubmenuColor, 0, 0,
                (),
                (
                    MenuBuilder.Button(*subMenuButtonSize, *world1ButtonColor, buttonFontSize, 'Next', 0, 11, 11),
                    MenuBuilder.Button(*subMenuButtonSize, *world1ButtonColor, buttonFontSize, 'Back', 0, 1),
                ),
                ()
            ),
        ),
    ),

    # Level 1-2 menu
    MenuBuilder.Menu(*mainMenuSize, *world1Color, 11, mainMenuDirection,
        (
            MenuBuilder.Text(*menuTextSize, *menuTextColor, menuTextFontSize, 'Level 2'),
        ),
        (
            MenuBuilder.Button(*mainMenuButtonSize, *world1ButtonColor, buttonFontSize, 'Play', 1, 2),
        ),
        (
            MenuBuilder.Menu(*subMenuSize, *world1SubmenuColor, 0, subMenuDirection,
                (),
                (
                    MenuBuilder.Button(*subMenuButtonSize, *world1ButtonColor, buttonFontSize, 'Prev', 0, 10),
                    MenuBuilder.Button(*subMenuButtonSize, *world1ButtonColor, buttonFontSize, 'Back', 0, 1),
                    MenuBuilder.Button(*subMenuButtonSize, *world1ButtonColor, buttonFontSize, 'Next', 0, 12, 12),
                ),
                ()
            ),
        ),
    ),
)


text = MenuBuilder.getLineNumber(menus)
with open('mainMenu.menuItem', 'w') as file:
    file.write(str(len(menus)) + text)
