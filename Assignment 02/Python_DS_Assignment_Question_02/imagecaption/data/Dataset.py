#Imports
import jsonlines
from PIL import Image
import os
import numpy as np

class Dataset(object):
    '''
        A class for the dataset that will return data items as per the given index
    '''

    def __init__(self, annotation_file, transforms=None):
        '''
            Arguments:
            annotation_file: path to the annotation file
            transforms: list of transforms (class instances)
                        For instance, [<class 'RandomCrop'>, <class 'Rotate'>]
        '''
        self.annotation_file = annotation_file
        self.transforms = transforms
        self.data = []

        with jsonlines.open(annotation_file, 'r') as reader:
            for obj in reader:
                self.data.append(obj)
        
    def __len__(self):
        '''
            return the number of data points in the dataset
        '''
        return len(self.data)

    def __getitem__(self, idx):
        '''
            return the data items for the index idx as an object
        '''
        item = self.data[idx]
        image_path = os.path.join('./data/imgs/', os.path.basename(item['file_name']))
        image = Image.open(image_path)
        if self.transforms:
            for transform in self.transforms:
                image = transform.transform(image)
        return image
    
    def __getann__(self, idx):
        '''
            return the annotations for the index idx as a dictionary object
        '''
        return self.data[idx]

    def __transformitem__(self, path):
        '''
            return transformed PIL Image object for the image in the given path
        '''
        image = Image.open(path)
        if self.transforms:
            for transform in self.transforms:
                image = transform(image)
                print(f"Performed Transition : {transform}")

        return image