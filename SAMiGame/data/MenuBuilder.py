def getLineNumber(items):
    item_texts    = [i.getText() for i in items]
    item_texts    = [''] + [str(i.count('\n')+1)+'\n'+i for i in item_texts]
    return '\n'.join(item_texts)

class MenuItem:
    def __init__(self, width, height, red, green, blue):
        self.width = width
        self.height = height
        self.red = red
        self.green = green
        self.blue = blue
    def getText(self):
        return f'{self.width}\n{self.height}\n{self.red}\n{self.green}\n{self.blue}'

class Menu(MenuItem):
    def __init__(self, width, height, red, green, blue, ID, direction, texts, buttons, submenus):
        super().__init__(width, height, red, green, blue)
        self.id = ID
        self.direction = direction
        self.noText = len(texts)
        self.noButton = len(buttons)
        self.noSubmenu = len(submenus)
        self.texts = texts
        self.buttons = buttons
        self.submenus = submenus
        
    def getText(self):
        return super().getText()+f'\n{self.id}\n{self.direction}\n{self.noText}\n{self.noButton}\n{self.noSubmenu}'+self.getSubitemText()
    
    def getSubitemText(self):\
        return getLineNumber(self.texts) + getLineNumber(self.buttons) + getLineNumber(self.submenus)
        

class Text(MenuItem):
    def __init__(self, width, height, red, green, blue, fontSize, text):
        super().__init__(width, height, red, green, blue)
        self.fontSize = fontSize
        self.text = text
    def getText(self):
        return super().getText()+f'\n{self.fontSize}\n{self.text}'

class Button(Text):
    def __init__(self, width, height, red, green, blue, fontSize, text, actionType, actionArg):
        super().__init__(width, height, red, green, blue, fontSize, text)
        self.actionType = actionType
        self.actionArg = actionArg
    def getText(self):
        return super().getText()+f'\n{self.actionType}\n{self.actionArg}'





def getIntFromInput(msg):
    while True:
        try:
            return int(input(msg))
        except KeyboardInterrupt:
            raise KeyboardInterrupt()
        except:
            pass

def getTextFromInput(i):
    print(f'Text {i}: ')
    width      = getIntFromInput(f'Width of Text {i}: ')
    height     = getIntFromInput(f'Height of Text {i}: ')
    red        = getIntFromInput(f'Red channel for background of Text {i}: ')
    green      = getIntFromInput(f'Green channel for background of Text {i}: ')
    blue       = getIntFromInput(f'Blue channel for background of Text {i}: ')
    fontSize   = getIntFromInput(f'Font size of Text {i}: ')
    text       = input(f'Text of Text {i}: ')
    return Text(width, height, red, green, blue, fontSize, text)

def getButtonFromInput(i):
    print(f'Button {i}: ')
    width      = getIntFromInput(f'Width of Button {i}: ')
    height     = getIntFromInput(f'Height of Button {i}: ')
    red        = getIntFromInput(f'Red channel for background of Button {i}: ')
    green      = getIntFromInput(f'Green channel for background of Button {i}: ')
    blue       = getIntFromInput(f'Blue channel for background of Button {i}: ')
    fontSize   = getIntFromInput(f'Font size of Button {i}: ')
    text       = input(f'Text of Button {i}: ')
    actionType = getIntFromInput(f'Action type of Button {i} (Switch menu=0): ')
    actionArg  = getIntFromInput(f'Action argument of Button {i}: ')
    return Button(width, height, red, green, blue, fontSize, text, actionType, actionArg)
    

def getMenuFromInput(i):
    print(f'Menu {i}: ')
    width      = getIntFromInput(f'Width of Menu {i}: ')
    height     = getIntFromInput(f'Height of Menu {i}: ')
    red        = getIntFromInput(f'Red channel for background of Menu {i}: ')
    green      = getIntFromInput(f'Green channel for background of Menu {i}: ')
    blue       = getIntFromInput(f'Blue channel for background of Menu {i}: ')
    ID         = int(i.rstrip().split('.')[-1])
    print('id is: ', ID)
    direction  = getIntFromInput(f'Direction of Menu {i} (Left=0, Right=1, Up=2, Down=3): ')
    number     = getIntFromInput(f'Number of text boxes for Menu {i}: ')
    texts      = [getTextFromInput(f'{i}.{j}') for j in range(number)]
    number     = getIntFromInput(f'Number of buttons for Menu {i}: ')
    buttons    = [getButtonFromInput(f'{i}.{j}') for j in range(number)]
    number     = getIntFromInput(f'Number of menus for Menu {i}: ')
    submenus   = [getMenuFromInput(f'{i}.{j}') for j in range(number)]
    return Menu(width, height, red, green, blue, ID, direction, texts, buttons, submenus)

def getRawMenusFromInput(noMenus):
    ms = [getMenuFromInput(str(i)).getText() for i in range(noMenus)]
    return [str(i.count('\n')+1)+'\n'+i for i in ms]





def getFormattedTemplateFromInput():
    width      = getIntFromInput('Width: ')
    height     = getIntFromInput('Height: ')
    red        = getIntFromInput('Red channel: ')
    green      = getIntFromInput('Green channel: ')
    blue       = getIntFromInput('Blue channel: ')
    return width, height, red, green, blue

def getFormattedTextTemplateFromInput():
    fontSize = getIntFromInput('Font size: ')
    return getFormattedTemplateFromInput()+(fontSize,)

def getFormattedTextFromInput(i, textFormat):
    text       = input(f'Text of Text {i}: ')
    return Text(textFormat[0], textFormat[1], textFormat[2], textFormat[3], textFormat[4], textFormat[5], text)

def getFormattedButtonFromInput(i, buttonFormat):
    text       = input(f'Text of Button {i}: ')
    actionType = getIntFromInput(f'Action type of Button {i} (Switch menu=0): ')
    actionArg  = getIntFromInput(f'Action argument of Button {i}: ')
    return Button(buttonFormat[0], buttonFormat[1], buttonFormat[2], buttonFormat[3], buttonFormat[4], buttonFormat[5],
                  text, actionType, actionArg)

def getFormattedMenuFromInput(i, menuFormat):
    print(f'Menu {i}: ')
    ID         = int(i.rstrip().split('.')[-1])
    direction  = getIntFromInput(f'Direction of Menu {i} (Left=0, Right=1, Up=2, Down=3): ')
    
    number     = getIntFromInput(f'Number of text boxes for Menu {i}: ')
    if number>0:
        print(f'Formatting for texts in Menu {i}: ')
        textFormat = getFormattedTextTemplateFromInput()
    texts      = [getFormattedTextFromInput(f'{i}.{j}', textFormat) for j in range(number)]
    
    number     = getIntFromInput(f'Number of buttons for Menu {i}: ')
    if number>0:
        print(f'Formatting for buttons in Menu {i}: ')
        buttonFormat = getFormattedTextTemplateFromInput()
    buttons    = [getFormattedButtonFromInput(f'{i}.{j}', buttonFormat) for j in range(number)]
    
    number     = getIntFromInput(f'Number of menus for Menu {i}: ')
    if number>0:
        print(f'Formatting for menus in Menu {i}: ')
        menuFormat = getFormattedTemplateFromInput()
    submenus   = [getFormattedMenuFromInput(f'{i}.{j}', menuFormat) for j in range(number)]
    
    return Menu(menuFormat[0], menuFormat[1], menuFormat[2], menuFormat[3], menuFormat[4], ID, direction, texts, buttons, submenus)

def getFormattedMenusFromInput(noMenus):
    print('Formatting for menus: ')
    menuFormat = getFormattedTemplateFromInput()
    
    ms = [getFormattedMenuFromInput(str(i), menuFormat).getText() for i in range(noMenus)]
    return [str(i.count('\n')+1)+'\n'+i for i in ms]





if __name__ == '__main__':
    noMenus = getIntFromInput('How many menus (number)? ')
    
    ms = getFormattedMenusFromInput(noMenus)
    filename = input('Output filename: ')
    with open(filename, 'w') as file:
        file.write(str(len(ms))+'\n'+'\n'.join(ms))
    
