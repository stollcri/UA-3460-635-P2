# Project 2: SVD & PCA
- Christopher Stoll
- Michael Crouse



## Singular Value Decomposition

![SVD](./img/SVD.png)



## SVD for Image Compression -- Example 1
### The first eigenvalue
![SVD k1](./img/RCA_Indian_Head-k1.png)



## SVD for Image Compression -- Example 1
### The second eigenvalue
![SVD k2](./img/RCA_Indian_Head-k1-j1.png)



## SVD for Image Compression -- Example 1
### The third eigenvalue
![SVD k3](./img/RCA_Indian_Head-k1-j2.png)



## SVD for Image Compression -- Example 1
### The first 4 eigenvalues combined
![SVD k1-4](./img/RCA_Indian_Head-k4.png)



## SVD for Image Compression -- Example 1
### The first 16 eigenvalues combined
![SVD k1-16](./img/RCA_Indian_Head-k16.png)



## SVD for Image Compression -- Example 1
### The first 64 eigenvalues combined
![SVD k1-64](./img/RCA_Indian_Head-k64.png)



## SVD for Image Compression -- Example 1
### The original image
![Original Image](./img/RCA_Indian_Head.png)



## SVD for Image Compression -- Example 2
### The first eigenvalue
![SVD k1](./img/SMPTE_Color_Bars-k1.png)



## SVD for Image Compression -- Example 2
### The first 2 eigenvalues combined
![SVD k2](./img/SMPTE_Color_Bars-k2.png)



## SVD for Image Compression -- Example 2
### The first 3 eigenvalues combined
![SVD k3](./img/SMPTE_Color_Bars-k3.png)



## SVD for Image Compression -- Example 2
### The original image
![Original Image](./img/SMPTE_Color_Bars.png)



## SVD for Image Compression
### Example 1
- 800x600
- k > **256** to be indistinguishable from original

### Example 2
- 672x504
- k > **3** to be indistinguishable from original



## Our Results

|                  | Size    | Original | Binary | Cmprs Rate | Cmprs Size | Cmprs Rate |
| ---------------- | ------- | -------- | ------ | ---------- | ---------- | ---------- |
| image.pgm        | 200x115 |      88K |    22K |        75% |        19K |        14% |
| IMG_2208.pgm     | 400x300 |     423K |   117K |        72% |        41K |        65% |
| IMG_2208-cut.pgm | 400x300 |     429K |   117K |        73% |        41K |        65% |
