#Imports
from PIL import Image

class RescaleImage(object):
    '''
        Rescales the image to a given size.
    '''

    def __init__(self, output_size):
        '''
            Arguments:
            output_size (tuple or int): Desired output size. If tuple, output is
            matched to output_size. If int, smaller of image edges is matched
            to output_size keeping aspect ratio the same.
        '''
        self.output_size = output_size

    def __call__(self, image):
        '''
            Arguments:
            image (numpy array or PIL image)

            Returns:
            image (numpy array or PIL image)

            Note: You do not need to resize the bounding boxes. ONLY RESIZE THE IMAGE.
        '''
        width, height = image.size

        print(f'Original Width : {width} & Original Height : {height}')

        if type(self.output_size)==int:
            if height > width:
                new_height, new_width = self.output_size * height / width, self.output_size
            else:
                new_height, new_width = self.output_size, self.output_size * width / height
        elif type(self.output_size)==str:
            x_factor = float(str[:-1])
            new_width, new_height = int(width*x_factor), int(height*x_factor)
        else:
            new_width, new_height = self.output_size

        print(f'Rescaled Width : {new_width} & Rescaled Height : {new_height}')

        new_width, new_height = int(new_width), int(new_height)
        temp_image = image.resize((new_width, new_height))

        return temp_image