import os
import pygame
from pygame.locals import *
import png, array
from tkinter import filedialog
from PIL import Image

pygame.init()


WIDTH = 800
HEIGHT = 800

SCREEN = pygame.display.set_mode((WIDTH,HEIGHT))

title = "PIKSEL --- FREE PNG PIXEL ART EDITOR ---"

MAINFONT = pygame.font.SysFont("freecomicsans", 32)

pygame.display.set_caption(title)
pygame.display.set_icon(pygame.image.load("gradient.png"))

clock = pygame.time.Clock()

def get_size(path):
    reader = png.Reader(path)
    info = reader.read()
    return info[3]["size"]

def get_input():
    selected = False

    box_rect = (WIDTH//2-150,HEIGHT//2-150,300,380)


    label_width = MAINFONT.render("WIDTH",True,((0,0,0)))
    label_width_pos = (box_rect[0]+box_rect[2]//2 -label_width.get_width()//2, box_rect[1]+20)
    text_box_width_rect  = (box_rect[0]+box_rect[2]//2 -100, label_width.get_height() + box_rect[1]+20, 200, 40)

    label_height = MAINFONT.render("HEIGHT",True,((0,0,0)))
    label_height_pos = (box_rect[0]+box_rect[2]//2 -label_height.get_width()//2, box_rect[1]+120)
    text_box_height_rect  = (box_rect[0]+box_rect[2]//2 -100, label_height.get_height() + box_rect[1]+120, 200, 40)

    label_name = MAINFONT.render("FILE NAME",True,((0,0,0)))
    label_name_pos = (box_rect[0]+box_rect[2]//2 -label_name.get_width()//2, box_rect[1]+120 + 100)
    text_box_name_rect  = (box_rect[0]+box_rect[2]//2 -100, label_name.get_height() + box_rect[1]+120 + 100, 200, 40)

    text_box_width_text  = "64"
    text_box_width_label = MAINFONT.render(text_box_width_text, True, ((160,150,170)))
    text_box_width_label_pos = (box_rect[0]+box_rect[2]//2 -100 + 20,label_width.get_height() + box_rect[1]+20+10)

    text_box_height_text  = "64"
    text_box_height_label = MAINFONT.render(text_box_height_text, True, ((160,150,170)))
    text_box_height_label_pos = (box_rect[0]+box_rect[2]//2 -100 + 20, label_height.get_height() + box_rect[1]+120+10)

    name_label = MAINFONT.render("", True, ((160,150,170)))
    name_label_pos = (box_rect[0]+box_rect[2]//2 -100 + 20, label_name.get_height() + box_rect[1]+120 + 110)

    submit_btn_label = MAINFONT.render("CREATE NEW", True, ((255,255,255)))
    submit_btn_rect = (WIDTH//2-submit_btn_label.get_width()//2,label_height.get_height() + box_rect[1]+120+10+100+100,submit_btn_label.get_width(),submit_btn_label.get_height())
    submit_btn_rect2 = (box_rect[0]+box_rect[2]//2 -100, label_width.get_height() + box_rect[1]+120 + 110+100, 200, 40)



    selected_box = None

    name = ""
    nums = ["1", "2", "3", "4", "5", "6", "7", "8", "9", "0"]

    def pop_string(string_to_pop):
        if len(string_to_pop) == 0 or len(string_to_pop) == 1:
            return ""
        else:
            save_string = ""
            for l in range(len(string_to_pop)-1):
                save_string += string_to_pop[l]
            return save_string
                

    while selected == False:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return -1
            if event.type == pygame.MOUSEBUTTONDOWN:
                pos = pygame.mouse.get_pos()
                r1 = pygame.rect.Rect(submit_btn_rect2)
                if r1.collidepoint(pos[0],pos[1]):
                    if len(text_box_width_text) != 0 and len(text_box_height_text) != 0 and name != "":
                        if text_box_width_text[0] != "0" and text_box_height_text[0] != "0":
                            width = int(text_box_width_text)
                            height = int(text_box_height_text)
                            img = []
                            for y in range(height):
                                row = ()
                                for x in range(width):
                                    row = row + (255, 255, 255)
                                img.append(row)
                            with open(name+'.png', 'wb') as f:
                                w = png.Writer(width, height, greyscale=False)
                                w.write(f, img)
                            return (int(text_box_width_text), int(text_box_height_text), name+".png")
                r = pygame.rect.Rect(text_box_width_rect)
                if r.collidepoint(pos[0],pos[1]):
                    selected_box = text_box_width_rect
                r2 = pygame.rect.Rect(text_box_height_rect)
                if r2.collidepoint(pos[0], pos[1]):
                    selected_box = text_box_height_rect
                r3 = pygame.rect.Rect(text_box_name_rect)
                if r3.collidepoint(pos[0],pos[1]):
                    selected_box = text_box_name_rect
            if event.type == pygame.KEYDOWN:
                if selected_box == text_box_width_rect:
                    if event.key != pygame.K_BACKSPACE and event.unicode in nums:
                        if len(text_box_width_text) < 3:
                            if len(text_box_height_text) == 1:
                                if text_box_height_text[0] != "0":
                                    text_box_width_text += event.unicode
                            else:
                                text_box_width_text += event.unicode
                    else:
                        if event.key == pygame.K_BACKSPACE:
                            text_box_width_text = pop_string(text_box_width_text)
                        else:
                            text_box_width_text = "64"
                    text_box_width_label = MAINFONT.render(text_box_width_text, True, ((160,150,170)))
                elif selected_box == text_box_height_rect:
                    if event.key != pygame.K_BACKSPACE and event.unicode in nums:
                        if len(text_box_height_text) < 3:
                            if len(text_box_height_text) == 1:
                                if text_box_height_text[0] != "0":
                                    text_box_height_text += event.unicode
                            else:
                                text_box_height_text += event.unicode
                    else:
                        if event.key == pygame.K_BACKSPACE:
                            text_box_height_text = pop_string(text_box_height_text)
                        else:
                            text_box_height_text = "64"
                    text_box_height_label = MAINFONT.render(text_box_height_text, True, ((160,150,170)))
                elif selected_box == text_box_name_rect:
                    if event.key != pygame.K_BACKSPACE:
                        name += event.unicode
                    else:
                        name = pop_string(name)
                    name_label = MAINFONT.render(name, True, ((160,150,170)))
            
        pygame.draw.rect(SCREEN, ((255,255,255)), box_rect)
        SCREEN.blit(label_width, label_width_pos)
        pygame.draw.rect(SCREEN, ((0,0,0)), text_box_width_rect, 5)
        SCREEN.blit(text_box_width_label, text_box_width_label_pos)
        SCREEN.blit(text_box_height_label, text_box_height_label_pos)
        pygame.draw.rect(SCREEN, ((0,0,0)), text_box_width_rect, 5)
        SCREEN.blit(label_height, label_height_pos)
        pygame.draw.rect(SCREEN, ((0,0,0)), text_box_height_rect, 5)
        
        pygame.draw.rect(SCREEN, ((255,0,0)), submit_btn_rect2)
        SCREEN.blit(submit_btn_label, (submit_btn_rect[0],submit_btn_rect[1]+10))

        pygame.draw.rect(SCREEN, ((0,0,0)), text_box_name_rect, 5)
        SCREEN.blit(label_name, label_name_pos)
        
        SCREEN.blit(name_label, name_label_pos)

        pygame.display.update()
        clock.tick(60)

def get_colour(cur_rgb):
    selected = False

    box_rect = (WIDTH//2-150,HEIGHT//2-150,300,380)


    label_width = MAINFONT.render("RED",True,((0,0,0)))
    label_width_pos = (box_rect[0]+box_rect[2]//2 -label_width.get_width()//2, box_rect[1]+20)
    text_box_width_rect  = (box_rect[0]+box_rect[2]//2 -100, label_width.get_height() + box_rect[1]+20, 200, 40)

    label_height = MAINFONT.render("GREEN",True,((0,0,0)))
    label_height_pos = (box_rect[0]+box_rect[2]//2 -label_height.get_width()//2, box_rect[1]+120)
    text_box_height_rect  = (box_rect[0]+box_rect[2]//2 -100, label_height.get_height() + box_rect[1]+120, 200, 40)

    label_name = MAINFONT.render("BLUE",True,((0,0,0)))
    label_name_pos = (box_rect[0]+box_rect[2]//2 -label_name.get_width()//2, box_rect[1]+120 + 100)
    text_box_name_rect  = (box_rect[0]+box_rect[2]//2 -100, label_name.get_height() + box_rect[1]+120 + 100, 200, 40)

    text_box_width_text  = str(cur_rgb[0])
    text_box_width_label = MAINFONT.render(text_box_width_text, True, ((160,150,170)))
    text_box_width_label_pos = (box_rect[0]+box_rect[2]//2 -100 + 20,label_width.get_height() + box_rect[1]+20+10)

    text_box_height_text  = str(cur_rgb[1])
    text_box_height_label = MAINFONT.render(text_box_height_text, True, ((160,150,170)))
    text_box_height_label_pos = (box_rect[0]+box_rect[2]//2 -100 + 20, label_height.get_height() + box_rect[1]+120+10)

    name_label = MAINFONT.render(str(cur_rgb[2]), True, ((160,150,170)))
    name_label_pos = (box_rect[0]+box_rect[2]//2 -100 + 20, label_name.get_height() + box_rect[1]+120 + 110)

    submit_btn_label = MAINFONT.render("PICK COLOR", True, ((255,255,255)))
    submit_btn_rect = (WIDTH//2-submit_btn_label.get_width()//2,label_height.get_height() + box_rect[1]+120+10+100+100,submit_btn_label.get_width(),submit_btn_label.get_height())
    submit_btn_rect2 = (box_rect[0]+box_rect[2]//2 -100, label_width.get_height() + box_rect[1]+120 + 110+100, 200, 40)



    selected_box = None

    name = str(cur_rgb[2])
    nums = ["1", "2", "3", "4", "5", "6", "7", "8", "9", "0"]

    def pop_string(string_to_pop):
        if len(string_to_pop) == 0 or len(string_to_pop) == 1:
            return ""
        else:
            save_string = ""
            for l in range(len(string_to_pop)-1):
                save_string += string_to_pop[l]
            return save_string
                

    while selected == False:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return -1
            if event.type == pygame.MOUSEBUTTONDOWN:
                pos = pygame.mouse.get_pos()
                r1 = pygame.rect.Rect(submit_btn_rect2)
                if r1.collidepoint(pos[0],pos[1]):
                    if len(text_box_width_text) != 0 and len(text_box_height_text) != 0 and int(name) >= 0:
                        if int(text_box_width_text[0]) >= 0 and int(text_box_height_text[0]) >= 0:
                            red = int(text_box_width_text)
                            green = int(text_box_height_text)
                            blue = int(name)
                            return (red, green, blue)
                r = pygame.rect.Rect(text_box_width_rect)
                if r.collidepoint(pos[0],pos[1]):
                    selected_box = text_box_width_rect
                r2 = pygame.rect.Rect(text_box_height_rect)
                if r2.collidepoint(pos[0], pos[1]):
                    selected_box = text_box_height_rect
                r3 = pygame.rect.Rect(text_box_name_rect)
                if r3.collidepoint(pos[0],pos[1]):
                    selected_box = text_box_name_rect
            if event.type == pygame.KEYDOWN:
                if selected_box == text_box_width_rect:
                    if event.key != pygame.K_BACKSPACE and event.unicode in nums:
                        if len(text_box_width_text) < 3:
                            if len(text_box_width_text) == 1:
                                if text_box_width_text[0] != "0":
                                    text_box_width_text += event.unicode
                            else:
                                text_box_width_text += event.unicode
                    else:
                        if event.key == pygame.K_BACKSPACE:
                            text_box_width_text = pop_string(text_box_width_text)
                        else:
                            text_box_width_text = "64"
                    text_box_width_label = MAINFONT.render(text_box_width_text, True, ((160,150,170)))
                elif selected_box == text_box_height_rect:
                    if event.key != pygame.K_BACKSPACE and event.unicode in nums:
                        if len(text_box_height_text) < 3:
                            if len(text_box_height_text) == 1:
                                if text_box_height_text[0] != "0":
                                    text_box_height_text += event.unicode
                            else:
                                text_box_height_text += event.unicode
                    else:
                        if event.key == pygame.K_BACKSPACE:
                            text_box_height_text = pop_string(text_box_height_text)
                        else:
                            text_box_height_text = "64"
                    text_box_height_label = MAINFONT.render(text_box_height_text, True, ((160,150,170)))
                elif selected_box == text_box_name_rect:
                    if event.key != pygame.K_BACKSPACE:
                        name += event.unicode
                    else:
                        name = pop_string(name)
                    name_label = MAINFONT.render(name, True, ((160,150,170)))
            
        pygame.draw.rect(SCREEN, ((255,255,255)), box_rect)
        SCREEN.blit(label_width, label_width_pos)
        pygame.draw.rect(SCREEN, ((0,0,0)), text_box_width_rect, 5)
        SCREEN.blit(text_box_width_label, text_box_width_label_pos)
        SCREEN.blit(text_box_height_label, text_box_height_label_pos)
        pygame.draw.rect(SCREEN, ((0,0,0)), text_box_width_rect, 5)
        SCREEN.blit(label_height, label_height_pos)
        pygame.draw.rect(SCREEN, ((0,0,0)), text_box_height_rect, 5)
        
        pygame.draw.rect(SCREEN, ((255,0,0)), submit_btn_rect2)
        SCREEN.blit(submit_btn_label, (submit_btn_rect[0],submit_btn_rect[1]+10))

        pygame.draw.rect(SCREEN, ((0,0,0)), text_box_name_rect, 5)
        SCREEN.blit(label_name, label_name_pos)
        
        SCREEN.blit(name_label, name_label_pos)

        pygame.display.update()
        clock.tick(60)
        
def get_pixels_list(file):
    reader = png.Reader(filename=file.path)
    w, h, pixels, metadata = reader.read_flat()
    pixel_width = 4 if metadata['alpha'] else 3
    index = pixel_width
    listt = [] 
    row = []
    for i in range(len(pixels)):
        index -= 1
        if index < 0:            
            listt.append(row)
            row = []
            index = pixel_width
        else:
            row.append(pixels[i])
    return listt

class FILE():
    def __init__(self, path, name, size):
        self.path = path
        self.file_name = name
        self.size = size
        self.pixels = get_pixels_list(self)
        #print(self.pixels)
    
def get_file():
    file = filedialog.askopenfile()
    if file != None:
        return FILE(file.name, file.name.split("/")[-1], get_size(file.name))
    return None

def create_new_file():
    read = get_input()
    if read != -1:
        return FILE(os.getcwd()+"/"+read[2], read[2], get_size(os.getcwd()+"/"+read[2]))
    return None

def change_pixel(point, file, rgb):
  reader = png.Reader(filename=file.path)
  w, h, pixels, metadata = reader.read_flat()
  pixel_byte_width = 4 if metadata['alpha'] else 3
  pixel_position = point[0] + point[1] * w
  new_pixel_value = (rgb[0], rgb[1], rgb[2], 0) if metadata['alpha'] else rgb
  pixels[
    pixel_position * pixel_byte_width :
    (pixel_position + 1) * pixel_byte_width] = array.array('B', new_pixel_value)

  output = open(file.path, 'wb')
  writer = png.Writer(w, h, **metadata)
  writer.write_array(output, pixels)
  output.close()

def read_pixel(point, file):
    im = Image.open(file.path)
    im = im.convert('RGB')
    x = point[0]
    y = point[1]
    pix = im.load()
    #print(pix[0,0])
    #print(pix[x,y])
    return pix[x,y]

class TEXT():
    def __init__(self, pos, text, font, rgb=((255,255,255))):
        self.pos = pos
        self.text = text
        self.font = font
        self.rgb = rgb

    def draw(self):
        SCREEN.blit(self.font.render(self.text, True, self.rgb), self.pos)

# points (x, y) som et koordinatsystem
pixels_changed = []

file_rect = (0,0,80,26)
new_file_rect = (80,0,80,26)
save_file_rect = (160,0,80,26)
colour_rect = (240,0,80,26)

offset = [WIDTH//4-13,HEIGHT//4-13]


current_rgb = (0,200,200)

upper_texts = [TEXT((13,7), "OPEN", pygame.font.SysFont("freecomicsansbold", 24)), TEXT((93,7), "NEW", pygame.font.SysFont("freecomicsansbold", 24)), TEXT((173,7), "SAVE", pygame.font.SysFont("freecomicsansbold", 24)),
TEXT((253,7), "COLOR", pygame.font.SysFont("freecomicsansbold", 24), (255-current_rgb[0],255-current_rgb[1],255-current_rgb[2]))]


rect_hovering_over = None

current_file = None

pixel_size = 5

class PIXEL():
    def __init__(self, pos, img_pos, rgb, size):
        self.world_pos = pos
        self.img_pos = img_pos
        self.colour = rgb
        self.size = size
        self.rect = (pos[0],pos[1],size,size)

    def draw(self):
        pygame.draw.rect(SCREEN, self.colour, (self.world_pos[0], self.world_pos[1], pixel_size, pixel_size))

pixels = []

end_message = " *"

run = True
while run:
    pos = pygame.mouse.get_pos()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
        if event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:
                #pos = pygame.mouse.get_pos()
                if pos[1] > 26:
                    if current_file != None:
                        for pix in pixels:
                            if pygame.rect.Rect(pix.rect).collidepoint(pos[0],pos[1]):
                                #change_pixel(pix.img_pos, current_file)
                                if pix not in pixels_changed:
                                    pix.colour = current_rgb
                                    pixels_changed.append(pix)
                                else:
                                    save = pix
                                    pix.colour = current_rgb
                                    pixels_changed[pixels_changed.index(save)] = pix
                else:
                    if pygame.rect.Rect(file_rect).collidepoint(pos[0],pos[1]):
                        current_file = get_file()
                        if current_file != None:
                            pygame.display.set_caption("PIKSEL --- " + current_file.file_name + " ---")
                            for y in range(current_file.size[1]):
                                for x in range(current_file.size[0]):                             
                                    pixels.append(PIXEL( (x*pixel_size + offset[0], y*pixel_size + offset[1]), (x,y), read_pixel((x,y), current_file), pixel_size ))
                    elif pygame.rect.Rect(new_file_rect).collidepoint(pos[0],pos[1]):
                        current_file = create_new_file()
                        if type(current_file) == int or current_file == -1 or current_file == None:
                            run = False
                        else:
                            pygame.display.set_caption("PIKSEL --- " + current_file.file_name + " ---")
                            for y in range(current_file.size[1]):
                                for x in range(current_file.size[0]):
                                    #pygame.draw.rect(SCREEN, current_file.pixels[y], (x*pixel_size + offset[0], y*pixel_size + offset[1], pixel_size, pixel_size))
                                    pixels.append(PIXEL( (x*pixel_size + offset[0], y*pixel_size + offset[1]), (x,y), ((255,255,255)), pixel_size ))
                    elif pygame.rect.Rect(save_file_rect).collidepoint(pos[0],pos[1]):
                        for pix in pixels_changed:
                            change_pixel(pix.img_pos, current_file, pix.colour)
                        pixels_changed = []
                    elif pygame.rect.Rect(colour_rect).collidepoint(pos[0], pos[1]):
                        current_rgb = get_colour(current_rgb)
                        upper_texts[-1].rgb = (255-current_rgb[0],255-current_rgb[1],255-current_rgb[2])
                        print(current_rgb)
            #print(pixels_changed)
                
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_z:
                pixel_size *= 2
                save = []
                for y in range(current_file.size[1]):
                    for x in range(current_file.size[0]):
                        save.append(PIXEL( (x*pixel_size + offset[0], y*pixel_size + offset[1]), (x,y), read_pixel((x,y), current_file), pixel_size ))
                pixels = save
            elif event.key == pygame.K_x:
                pixel_size = pixel_size // 2
                save = []
                for y in range(current_file.size[1]):
                    for x in range(current_file.size[0]):
                        print(read_pixel((x,y), current_file))
                        save.append(PIXEL( (x*pixel_size + offset[0], y*pixel_size + offset[1]), (x,y), read_pixel((x,y), current_file), pixel_size ))
                pixels = save

    SCREEN.fill((172,172,172))

    for pix in pixels:
        pix.draw()

    pygame.draw.rect(SCREEN, ((172,172,172)), (0,0,WIDTH,26))
    pygame.draw.line(SCREEN, ((0,0,0)), (0,26), (WIDTH,26) )

    if pos[1] < 26:
        if pygame.rect.Rect(file_rect).collidepoint(pos[0],pos[1]):
            pygame.draw.rect(SCREEN, ((255,0,0)), file_rect )
        else:
            pygame.draw.rect(SCREEN, ((0,0,0)), file_rect )
    else:
        pygame.draw.rect(SCREEN, ((0,0,0)), file_rect )


    if pygame.rect.Rect(new_file_rect).collidepoint(pos[0],pos[1]):
        pygame.draw.rect(SCREEN, ((255,0,0)), new_file_rect )
    else:
        pygame.draw.rect(SCREEN, ((0,0,0)), new_file_rect)

    if pygame.rect.Rect(save_file_rect).collidepoint(pos[0],pos[1]):
        pygame.draw.rect(SCREEN, ((255,0,0)), save_file_rect )
    else:
        pygame.draw.rect(SCREEN, ((0,0,0)), save_file_rect)
    
    if pygame.rect.Rect(colour_rect).collidepoint(pos[0],pos[1]):
        pygame.draw.rect(SCREEN, ((current_rgb)), colour_rect )
    else:
        pygame.draw.rect(SCREEN, (current_rgb), colour_rect)

    for text in upper_texts:
        text.draw()

    pygame.display.update()
    clock.tick(60)
