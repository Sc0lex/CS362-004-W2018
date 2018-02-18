#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "testUtils.h"

int main (int argc, char** argv) {
	printf("Hello world!\n");
	
	FailuresReport* pTest = NewReport();
	ReportFailure(pTest, "Failure 1", 1);
	ReportFailure(pTest, "Failure 1", 2);
	ReportFailure(pTest, "Failure 1", 3);
	ReportFailure(pTest, "Failure 1", 4);
	ReportFailure(pTest, "Failure 1", 5);
	ReportFailure(pTest, "Failure 1", 6);
	ReportFailure(pTest, "Failure 1", 7);
	ReportFailure(pTest, "Failure 1", 8);
	ReportFailure(pTest, "Failure 2", 1);
	ReportFailure(pTest, "Failure 2", 2);
	ReportFailure(pTest, "Failure 2", 3);
	ReportFailure(pTest, "Failure 3", 1);
	ReportFailure(pTest, "Failure 3", 2);
	ReportFailure(pTest, "Failure 3", 3);
	ReportFailure(pTest, "Failure 3", 4);
	ReportFailure(pTest, "Failure 1", 1);
	
	PrintFailures(pTest);
	
	DeleteReport(pTest);
}