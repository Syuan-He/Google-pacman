import cv2
import numpy as np
from PIL import Image 
import copy
import os

def txt_strtonum_feed(filename):
    with open(filename) as file:
        lines = file.readlines()
    array_2d = [list(map(int, line.strip().split())) for line in lines]
    return array_2d

def img_to_piece(filename):
    if not os.path.isdir("image"):
        os.mkdir("image")
    else:
        return
    img = cv2.imread(filename, 1)
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    data = np.asarray(img)
    print(data.shape)

    img_data = []
    for r in range(0,img.shape[0],8):
        for c in range(0,img.shape[1],8):
            d = np.asarray(img[r:r+8, c:c+8,:])
            b = True
            for i_t in img_data:
                if np.array_equal(i_t, d):
                    b = False
            if b:
                img_data.append(d)
    i = 0
    for i_t in img_data:
        im = Image.fromarray(i_t.astype('uint8'))
        im = im.save(f'image/{i}.bmp')
        i += 1

def detect_img(A, i, j):
    map_t = {
        "1011": 7,
        "1101": 17,
        "1110": 15,
        "0111": 18,
        "1001": 9,
        "1100": 22,
        "0110": 16,
        "0011": 8,
    }
    if A[i][j] == 0:
        return 4
    else:
        str = ""
        if A[i][j + 1] == 1:
            str += "1"
        else:
            str += "0"
        if A[i - 1][j] == 1:
            str += "1"
        else:
            str += "0"
        if A[i][j - 1] == 1:
            str += "1"
        else:
            str += "0"
        if A[i + 1][j] == 1:
            str += "1"
        else:
            str += "0"
        try:
            return map_t[str]
        except KeyError:
            return 4

def turn_pic(data_txt):
    data_txt_copy = copy.deepcopy(data_txt)
    for r in range(len(data_txt)):
        for c in range(len(data_txt[0])):
            if r == 0 and c == 0:
                data_txt_copy[r][c] = 0
            elif r == 0 and c == len(data_txt[0]) - 1:
                data_txt_copy[r][c] = 2
            elif r == len(data_txt) - 1 and c == 0:
                data_txt_copy[r][c] = 79
            elif r == len(data_txt) - 1 and c == len(data_txt[0]) - 1:
                data_txt_copy[r][c] = 82
            elif r == 0:
                if data_txt[r + 1][c] == 1:
                    if data_txt[r + 1][c + 1] == 1 and data_txt[r + 1][c - 1] == 1:
                        data_txt_copy[r][c] = 4
                    elif data_txt[r + 1][c + 1] == 1 and data_txt[r + 1][c - 1] == 0:
                        data_txt_copy[r][c] = 84
                    elif data_txt[r + 1][c + 1] == 0 and data_txt[r + 1][c - 1] == 1:
                        data_txt_copy[r][c] = 83
                else:
                    data_txt_copy[r][c] = 1
            elif r == len(data_txt) - 1:
                if data_txt[r - 1][c] == 1:
                    if data_txt[r - 1][c + 1] == 1 and data_txt[r - 1][c - 1] == 1:
                        data_txt_copy[r][c] = 4
                    elif data_txt[r - 1][c + 1] == 1 and data_txt[r - 1][c - 1] == 0:
                        data_txt_copy[r][c] = 80
                    elif data_txt[r - 1][c + 1] == 0 and data_txt[r - 1][c - 1] == 1:
                        data_txt_copy[r][c] = 81
                else:
                    data_txt_copy[r][c] = 26
            elif c == 0:
                if data_txt[r][c + 1] == 1:
                    if data_txt[r - 1][c + 1] == 1 and data_txt[r + 1][c + 1] == 1:
                        data_txt_copy[r][c] = 4
                    elif data_txt[r - 1][c + 1] == 1 and data_txt[r + 1][c + 1] == 0:
                        data_txt_copy[r][c] = 86
                    elif data_txt[r - 1][c + 1] == 0 and data_txt[r + 1][c + 1] == 1:
                        data_txt_copy[r][c] = 85
                else:
                    data_txt_copy[r][c] = 3
            elif c == len(data_txt[0]) - 1:
                if data_txt[r][c - 1] == 1:
                    if data_txt[r - 1][c - 1] == 1 and data_txt[r + 1][c - 1] == 1:
                        data_txt_copy[r][c] = 4
                    elif data_txt[r - 1][c - 1] == 1 and data_txt[r + 1][c - 1] == 0:
                        data_txt_copy[r][c] = 87
                    elif data_txt[r - 1][c - 1] == 0 and data_txt[r + 1][c - 1] == 1:
                        data_txt_copy[r][c] = 88
                else:
                    data_txt_copy[r][c] = 5
            else:
                data_txt_copy[r][c] = detect_img(data_txt, r, c)
    A = np.empty((len(data_txt), len(data_txt[0])), dtype=object)
    for r in range(len(data_txt)):
        for c in range(len(data_txt[0])):
            img = cv2.imread(f'image/{data_txt_copy[r][c]}.bmp', 1)
            img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
            data = np.asarray(img)
            A[r][c] = data

    result = np.vstack([np.hstack(row) for row in A])
    im = Image.fromarray(result.astype('uint8'))
    return im, data_txt_copy

def revise_pic(array_o, array_t):
    map_t = {
        "11111110": 9,
        "10111111": 22,
        "11101111": 16,
        "11111011": 8,
    }
    dir = [
        [0, 1],
        [-1, 1],
        [-1, 0],
        [-1, -1],
        [0, -1],
        [1, -1],
        [1, 0],
        [1, 1],
    ]

    for i in range(1, len(array_o) - 1):
        for j in range(1, len(array_o[0]) - 1):
            str = ""
            for d in dir:
                if array_o[i + d[0]][j + d[1]] == 1:
                    str += "1"
                else:
                    str += "0"
            try:
                array_t[i][j] = map_t[str]
            except KeyError:

                continue
    
    A = np.empty((len(array_o), len(array_o[0])), dtype=object)
    for r in range(len(array_o)):
        for c in range(len(array_o[0])):
            img = cv2.imread(f'image/{array_t[r][c]}.bmp', 1)
            img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
            data = np.asarray(img)
            A[r][c] = data

    result = np.vstack([np.hstack(row) for row in A])
    im = Image.fromarray(result.astype('uint8'))
    return im, array_t

def pic_to_txt(filename, output_name):
    img = cv2.imread(filename)
    grayImg = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    data = np.asarray(grayImg)

    res = np.empty((len(data), len(data[0])), dtype=int)
    for i in range(len(data)):
        for j in range(len(data[0])):
            if data[i][j] == 255:
                res[i][j] = 0
            else:
                res[i][j] = 1
    with open(output_name, "w") as f:
        for row in res:
            row_str = " ".join([str(x) for x in row]).replace("[]", "").replace("]", "").replace(",", "")
            f.write(row_str + "\n")
    return output_name


path_reference_image = "googleMap0.bmp"
path_image_want_to_turn = "test.bmp" #use black as wall, white as way
name_image_txt = "test.txt" #the name you want your image's txt file be 
name_image = "compile_test.bmp" #the name of the finial image 

img_to_piece(path_reference_image)
d = pic_to_txt(path_image_want_to_turn, name_image_txt)
data_txt = txt_strtonum_feed(d)
im, res = turn_pic(data_txt)
im, res_r = revise_pic(data_txt, res)
im.show()
# im = im.save(name_image)