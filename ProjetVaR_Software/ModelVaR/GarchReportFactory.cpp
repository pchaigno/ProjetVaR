#include "GarchReportFactory.h"

/**
 * @brief Constructor
 * @param docxPath The location of the DOCX file on the disk.
 * @param pdfPath The location of the PDF file on the disk.
 */
GarchReportFactory::GarchReportFactory(string docxPath, string pdfPath):
	ReportFactory(docxPath, pdfPath) {

}

/**
 * @brief Generates the DOCX file using a DocxGenerator.
 * @return The DocxGenerator used.
 */
DocxGenerator& GarchReportFactory::generateDOCX() {
	// TODO
	return this->docxGenerator;
}