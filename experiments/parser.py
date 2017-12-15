#!/bin/python
import argparse
import csv 
import os.path

description='''
This file segregates kernel statistics according to process
Sample run
find . -name "*.log" -exec python parser.py -i {} \;
'''
helpFile='''
input a file
T
'''
def main(args):
	inputFile = args.i
	outputFile = args.o
	exists = os.path.isfile(outputFile)
	out = open(outputFile,'a')
	csv_writer = csv.writer(out)
	index = ["inputFile", "proc_name", "kernel_entry", "kernel_Time", "PGFLT_Count", "PGFLT_Minor", "PGFLT_Major",\
		"PGFLT_Time", "PGFLT_Minor_Time", "PGFLT_Major_Time", "BRK_Count", "BRK_Time", "MMAP_count", "MMAP_Time",\
		"Mremap_count", "Mremap_Time", "Total_Time", "O0", "O1", "O2", "O3", "O4", "O5", "O6", "O7", "O8", "O9",\
		 "Total_Kernel_Allocation", "Total_User_Allocation"]
	if False == exists: 
		csv_writer.writerow(index)
	with open(inputFile, 'r') as f:
		for x in f:
			if (-1 != x.find("----------------------------#")):
				if (0 == len(x.strip())):
					continue
				proc_name = x.split("--")[0].split("] ")[1].strip()
				if proc_name != "java" and proc_name != "python3.4" and proc_name != "python":
					continue
				next(f)
				kernel_entry = next(f).split(":")[-1].strip()
				if kernel_entry == '0':
					continue
				kernel_Time = next(f).split(":")[-1].strip()
				PGFLT_Count  = next(f).split(":")[-1].strip()
				PGFLT_Minor = next(f).split(":")[-1].strip()
				PGFLT_Major = next(f).split(":")[-1].strip()
				PGFLT_Time = next(f).split(":")[-1].strip()
				PGFLT_Minor_Time = next(f).split(":")[-1].strip()
				PGFLT_Major_Time = next(f).split(":")[-1].strip()
				BRK_Count = next(f).split(":")[-1].strip()
				BRK_Time = next(f).split(":")[-1].strip()
				MMAP_count = next(f).split(":")[-1].strip()
				MMAP_Time = next(f).split(":")[-1].strip()
				Mremap_count = next(f).split(":")[-1].strip()
				Mremap_Time = next(f).split(":")[-1].strip()
				Total_Time = next(f).split(":")[-1].strip()
				Order = next(f).split("] ")[1].strip()
				nextLine = next(f)
				while (-1 == nextLine.find("Total Kernel Allocation")):
					Order += nextLine.split("] ")[1].strip()
					nextLine = next(f)
				for i in Order.split(","):
					if(0 == len(i)):
						continue
					locals()[i.split(":")[-2].strip()] = i.split(":")[-1].strip()
				Total_Kernel_Allocation = nextLine.split(":")[1].strip().split(" ")[0].strip()
				Total_User_Allocation = next(f).split(":")[1].strip().split(" ")[0].strip()
				row = []
				for i in index:
					if i not in locals().keys():
						row.append(0)
					else:
						row.append(locals()[i])
				csv_writer.writerow(row)
				

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description=description)
	parser.add_argument('-i', help=helpFile)
	parser.add_argument('-o', default="out.csv", help=helpFile)
	args = parser.parse_args()
	main(args)
