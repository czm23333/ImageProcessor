# ImageProcessor
Image encrypt/decrypt tool

## What is it?
It's a tool that can help you turn your images to images that seem to be full of noise and turn it back then.  
**Notice: Results may be not as good as you expect if you use this tool on highly compressed image formats(jpeg, etc.).**
<details>
  <summary><b>Examples</b></summary>

  <img src="https://s1.ax1x.com/2020/08/02/aJyrT0.png" alt="Original image" title="Original image">
  <img src="https://s1.ax1x.com/2020/08/02/aJy2pF.png" alt="Encrypted image" title="Encrypted image">
  <img src="https://s1.ax1x.com/2020/08/02/aJy4mR.png" alt="Decrypted image" title="Decrypted image">
</details>

## How it works?
It uses linear congruential generator and a specific seed(CRC32 of the key) to generate a series of numbers.
Then it adds these numbers to the RGB values of every pixel in the image and save the result.

## How to use it?
### 1. Open image file using this program
![Open a file](https://s1.ax1x.com/2020/08/02/aJUeJA.png "Open a file")
### 2. Choose the method to use
![Choose a method](https://s1.ax1x.com/2020/08/02/aJaw9A.png "Choose a method")
### 3. Enter your key
![Enter your key](https://s1.ax1x.com/2020/08/02/aYGcIs.png "Enter your key")
### 4. Wait patiently
### 5. Finish
![Finish](https://s1.ax1x.com/2020/08/02/aYGTZ4.png "Finish")  
**Notice: The result will be written back to the original file.**

## How can I know which method I should use?
***Add(1)*** and ***Sub(0)*** are the opposite of each other.  
While using the same key, if you do same number of ***Add*** and ***Sub*** operation, you'll get the **original** image.
