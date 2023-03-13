#Imports
from imagecaption.model import ImageCaptioningModel
from imagecaption.data import Dataset, Download
#from imagecaption.data.transforms import FlipImage, RescaleImage, BlurImage, CropImage, RotateImage
from imagecaption.data.transforms.FlipImage import FlipImage
from imagecaption.data.transforms.BlurImage import BlurImage
from imagecaption.data.transforms.RescaleImage import RescaleImage
from imagecaption.data.transforms.CropImage import CropImage
from imagecaption.data.transforms.RotateImage import RotateImage
import numpy as np
import os
from PIL import Image

#additional imports to write captions alongside transformed images
from PIL import ImageFont, ImageDraw
import textwrap

def experiment(annotation_file, captioner, transforms, outputs, name, flag=1):
    '''
        Function to perform the desired experiments

        Arguments:
        annotation_file: Path to annotation file
        captioner: The image captioner
        transforms: List of transformation classes
        outputs: Path of the output folder to store the images
    '''

    # Create the instances of the dataset and download
    dataset = Dataset.Dataset(annotation_file, transforms)
    download = Download.Download()

    if not flag:
        # Print image names and their captions from the annotation file using the dataset object
        for idx in range(len(dataset)):
            annotation = dataset.__getann__(idx)
            print(f"Image Name: {annotation['file_name']}")
            print(f"Caption: {annotation['captions']}")
            print("\n")

        # Download images to ./data/imgs/ folder using the download object
        for idx in range(len(dataset)):
            annotation = dataset.__getann__(idx)
            download_path = os.path.join('./data/imgs/', os.path.basename(annotation['file_name']))
            download(download_path, annotation['url'])
        return

    # Transform the required image (roll number mod 10) and save it separately
    print()
    index = 2

    annotation = dataset.__getann__(index)
    image_path = os.path.join('./data/imgs/', os.path.basename(annotation['file_name']))
    transformed_img = dataset.__transformitem__(image_path)
    if outputs is None:
        outputs = './outputs'
    
    # save_path = os.path.join(outputs, 'output_imgs', os.path.basename(annotation['file_name']))
    save_path = os.path.join(outputs, 'output_imgs', name)
    transformed_img.save(save_path)
    #transformed_img.show()

    # Get the predictions from the captioner for the above saved transformed image
    print()
    number_of_captions = 3

    # prediction = captioner(os.path.join(outputs, 'output_imgs', os.path.basename(annotation['file_name'])), number_of_captions)
    prediction = captioner(os.path.join(outputs, 'output_imgs', name), number_of_captions)
    print(f"Generated Captions for {name}: {prediction}")

    # below part id to add captions to right of transformed image

    width, height = transformed_img.size
    space, size, vertical_typer = 400, 20, 20
    if height<200:
        size = 10
        space = 200
        vertical_typer = 10
    captioned_image = Image.new("RGB", (width+space+10, height+20), color="white")
    offset = (10,10)
    captioned_image.paste(transformed_img, offset)

    text = "Generated Captions : "
    typer = ImageDraw.Draw(captioned_image)
    shape = [(width+20, 10), (width+space, height+10)]
    typer.rectangle(shape, fill="white", outline="black")
    font = ImageFont.truetype("calibri.ttf", size=size)
    typer.text((width+27, vertical_typer), text, font=font, fill="black")
    typer.text((width+27, vertical_typer+1), "_________________", font=font, fill="black")

    inc1, inc2 = 10, 30
    if height<200:
        inc1, inc2 = 5, 15

    for caption in prediction:
        vertical_typer+=inc1
        caption = "'" + caption + "'"
        para = textwrap.wrap(caption, width = 40)
        for line in para: 
            vertical_typer+= inc2
            typer.text((width+35, vertical_typer), line, font=font, fill="black")

    # save_path = os.path.join(outputs, 'output_imgs_with_captions', os.path.basename(annotation['file_name']))
    save_path = os.path.join(outputs, 'output_imgs_with_captions', name)
    captioned_image.save(save_path)
    #captioned_image.show()
    print()


def main():
    captioner = ImageCaptioningModel()
    output_path = './outputs'

    experiment('./data/annotations.jsonl', captioner, [], output_path, 0, 0)

    image = Image.open('./data/imgs/2.jpg')
    width, height = image.size

    #part a
    experiment('./data/annotations.jsonl', captioner, [], output_path, 'a) Original Image.jpg')

    #part b
    experiment('./data/annotations.jsonl', captioner, [FlipImage()], output_path, 'b) Horizontally flipped Image.jpg')

    #part c
    experiment('./data/annotations.jsonl', captioner, [BlurImage(1)], output_path, 'c) Blurred Image.jpg')

    #part d
    experiment('./data/annotations.jsonl', captioner, [RescaleImage(2*height)], output_path, 'd) Twice Rescaled Image.jpg')
    # or
    # experiment('./data/annotations.jsonl', captioner, [RescaleImage((2*width, 2*height))], output_path, 'd. Twice Rescaled Image.jpg')
    # or
    # experiment('./data/annotations.jsonl', captioner, [RescaleImage('2x')], output_path, 'd. Twice Rescaled Image.jpg')

    #part e
    experiment('./data/annotations.jsonl', captioner, [RescaleImage(height//2)], output_path, 'e) Half Rescaled Image.jpg')
    # or
    # experiment('./data/annotations.jsonl', captioner, [RescaleImage((0.5*width, 0.5*height))], output_path, 'e. Half Rescaled Image.jpg')
    # or
    # experiment('./data/annotations.jsonl', captioner, [RescaleImage('0.5x')], output_path, 'e. Half Rescaled Image.jpg')

    #part f
    experiment('./data/annotations.jsonl', captioner, [RotateImage(-90)], output_path, 'f) 90 Degree Right Rotated Image.jpg')

    #part g
    experiment('./data/annotations.jsonl', captioner, [RotateImage(45)], output_path, 'g) 45 Degree Left Rotated Image.jpg')


if __name__ == '__main__':
    main()