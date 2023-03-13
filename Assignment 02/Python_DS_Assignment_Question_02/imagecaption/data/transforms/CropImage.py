#Imports
from PIL import Image
import random

class CropImage(object):
    '''
        Performs either random cropping or center cropping.
    '''

    def __init__(self, shape, crop_type='center'):
        '''
            Arguments:
            shape: output shape of the crop (h, w)
            crop_type: center crop or random crop. Default: center
        '''
        self.shape = shape
        self.crop_type = crop_type

    def __call__(self, image):
        '''
            Arguments:
            image (numpy array or PIL image)

            Returns:
            image (numpy array or PIL image)
        '''
        width, height = image.size

        if self.crop_type == 'center':
            new_width, new_height = self.shape
            left = (width - new_width) // 2
            top = (height - new_height) // 2
    
        else:
            new_width, new_height = self.shape
            left = random.randint(0, width - new_width)
            top = random.randint(0, height - new_height)

        right = left + new_width
        bottom = top + new_height
        image = image.crop((left, top, right, bottom))

        return image