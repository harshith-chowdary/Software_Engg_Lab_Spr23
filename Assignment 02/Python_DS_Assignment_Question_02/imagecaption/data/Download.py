from PIL import Image
import requests
from io import BytesIO

class Download(object):
    '''
        A class for helping in dowloading the required images from the given url to the specified path
    '''

    def __call__(self, path, url):
        '''
            Arguments:
            path: download path with the file name
            url: required image URL
        '''
        response = requests.get(url)
        
        if response.status_code != requests.codes.ok:
            try:
                assert False, 'Status code error: {}.'.format(response.status_code)
            except:
                pass

        with Image.open(BytesIO(response.content)) as im:
            im.save(path)

        print('Image downloaded from url: {} and saved to: {}.'.format(url, path))
