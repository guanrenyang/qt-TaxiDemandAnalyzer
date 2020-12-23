# Taxi Demand Analyzer 

## Final project of CS241-Principles And Problem Solving 

### Summary

The project is the final project of CS241, Shanghai Jiao Tong University. One of the features of it is to analyze the Didi taxi order during 2016/11/01 to 2016/11/15. The other is to tell the **taxi cost**, **estimated time** and the **total distance** , as long as you input your **original address** and **destination**. It can help plot the some types of **lining charts** and **3DHistogram** as you like.  

### Building and Running

#### 1. Using or checking the program(on Windows)

You could use or check the program easily by:

1. Download the "TaxiDemandAnalyzer" folder
2. Double clicked the "TaxiDemandAnalyzer.exe" file

#### 2. Developing the program or running it on Mac or Linux

1. Install the "QtCreator" IDE

2. Open the "TaxiDemandAnalyzer.pro" file to open the project

3. Configure:

   ​	version of Qt : 5.15.1

   ​	c++ compiler : MinGW_64bit

4. Build and run the program in Qtcreator

### Functionalities

#### 1. Didi taxi demand analysis

#### A. Load Data

Click the "Load Data" button in the "File" tool bar in the up-left corner of the mainwindow and a new window will appear. You could select the data in any time span as you need.  If you won't change the time span, just click "Load" button and all of the data will be loaded by default.

Selecting window:

https://github.com/guanrenyang/Pictures-In-Markdown-Files/blob/master/%E5%B1%8F%E5%B9%95%E6%88%AA%E5%9B%BE%202020-12-23%20030449.png

Loading progress:

![屏幕截图 2020-12-23 030522](C:\Users\69540\Desktop\概统复习课\屏幕截图 2020-12-23 030522.png)



#### B. Line Chart

After loading data, click the "Fitting Line" button to open the "Line Chart" mode.

It could show the **total number of departure orders** and the **total fees of departure orders** over time. Furthermore, you could select in which time span you plot the data and **the plotting precision will change with the change of the time span you choose** 

Initial line chart![屏幕截图 2020-12-23 031217](C:\Users\69540\Desktop\概统复习课\屏幕截图 2020-12-23 031217.png)

After changing the time span:

![屏幕截图 2020-12-23 031250](C:\Users\69540\Desktop\概统复习课\屏幕截图 2020-12-23 031250.png)

### C. 3D Histogram

After loading data, click the "3D Histogram" button to open the "3D Histogram" mode.

 The mode show the **total departure orders** in one particular day and grid. You could scroll the mouse wheel to enlarge or shrink the graph and you could adjust the "rotate horizontally" and "rotate vertically" slider to change the 3D perspective of the graph. 

Check the "show second series" to set the histogram of fees visible.

You could see the data of one particular day by change the text of the "Select Grid Number" ComboBox.

All of the data:

![屏幕截图 2020-12-23 031148](C:\Users\69540\Desktop\概统复习课\屏幕截图 2020-12-23 031148.png)

One particular day:

![屏幕截图 2020-12-23 031204](C:\Users\69540\Desktop\概统复习课\屏幕截图 2020-12-23 031204.png)



#### 2. Order Information Predict

Input your **origin** and **destination** in the *Original City* and *Destination City* and click the *Navigation* button, the program will show the **taxi cost**,**estimated time ** and **total distance**. You could do this while data is loading. 

If your inputs are very ambiguous or illegal which result the program can't get the answer, it will remind you to "Please specity your address".

Predict succeed:

![屏幕截图 2020-12-23 031126](C:\Users\69540\Desktop\概统复习课\屏幕截图 2020-12-23 031126.png)

Prediction fail:

![屏幕截图 2020-12-23 031916](C:\Users\69540\Desktop\概统复习课\屏幕截图 2020-12-23 031916.png)

