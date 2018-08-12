from tqdm import tqdm #Used for file writing progress bar
import xlrd as xl
import sys

#Prop Spreadsheet MUST be sorted by column D (prop category) else this program starts putting the wrong category with the wrong prop!
file = 'GTAV Props 2.xlsm'
print("Opened Excel workbook\n")
book = xl.open_workbook(file)

prop_sheet = book.sheet_by_name("Props")
print("Found Props sheet and finding required columns\n")
prop_names = prop_sheet.col_values(0) #prop name used to spawn it
generated_prop_names = prop_sheet.col_values(1) #Given prop name (i.e Cabbage #3)
prop_category = prop_sheet.col_values(3) #Prop category


#Iterate over the number of rows in the spreadsheet
print("Attempting to print Prop data to file\n")
with open("props_ouput.txt", 'w') as f:
	for data in tqdm(range(prop_sheet.nrows)):
	#Print the required data for the Props header
		f.write("{ " + "\"" + prop_names[data] + "\", " + "\"" + generated_prop_names[data] + "\", " + "\"" + prop_category[data] + "\" }, " + "\n")
	print("Printed data to file!")
