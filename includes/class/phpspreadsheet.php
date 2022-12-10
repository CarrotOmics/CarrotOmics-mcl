<?php
/**
 * Dependencies of PhpSpreadsheet
 * Only the ones actually needed by MCL are loaded
 *
 */
$lib_path = mcl_get_config_setting('mcl_lib_dir');

// Dependencies of ZipStream
require_once("$lib_path/comparator/Comparator.php");
require_once("$lib_path/http-message/StreamInterface.php");
require_once("$lib_path/php-enum/PHPUnit/Comparator.php");
require_once("$lib_path/php-enum/Enum.php");

// ZipStream, a dependency of PhpSpreadsheet
require_once("$lib_path/ZipStream-PHP/Option/Archive.php");
require_once("$lib_path/ZipStream-PHP/Option/File.php");
require_once("$lib_path/ZipStream-PHP/Option/Method.php");
require_once("$lib_path/ZipStream-PHP/Option/Version.php");
require_once("$lib_path/ZipStream-PHP/Bigint.php");
require_once("$lib_path/ZipStream-PHP/Stream.php");
require_once("$lib_path/ZipStream-PHP/DeflateStream.php");
require_once("$lib_path/ZipStream-PHP/Exception.php");
require_once("$lib_path/ZipStream-PHP/File.php");
require_once("$lib_path/ZipStream-PHP/ZipStream.php");

// A dependency of PhpSpreadsheet
require_once("$lib_path/simple-cache/CacheInterface.php");

// PhpSpreadsheet
require_once("$lib_path/PhpSpreadsheet/Shared/OLE.php");
require_once("$lib_path/PhpSpreadsheet/Shared/OLERead.php");
require_once("$lib_path/PhpSpreadsheet/Spreadsheet.php");
require_once("$lib_path/PhpSpreadsheet/Exception.php");
require_once("$lib_path/PhpSpreadsheet/Chart/DataSeries.php");
require_once("$lib_path/PhpSpreadsheet/IComparable.php");
require_once("$lib_path/PhpSpreadsheet/Style/Supervisor.php");
require_once("$lib_path/PhpSpreadsheet/Shared/File.php");
require_once("$lib_path/PhpSpreadsheet/Shared/JAMA/Matrix.php");
require_once("$lib_path/PhpSpreadsheet/Shared/XMLWriter.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Exception.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Exception.php");
require_once("$lib_path/PhpSpreadsheet/IOFactory.php");
require_once("$lib_path/PhpSpreadsheet/Writer/IWriter.php");
require_once("$lib_path/PhpSpreadsheet/Writer/BaseWriter.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Csv.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xls.php");

// Writer
require_once("$lib_path/PhpSpreadsheet/Calculation/Token/Stack.php");
require_once("$lib_path/PhpSpreadsheet/Calculation/Calculation.php");
require_once("$lib_path/PhpSpreadsheet/Calculation/Functions.php");
require_once("$lib_path/PhpSpreadsheet/HashTable.php");
require_once("$lib_path/PhpSpreadsheet/Style/Borders.php");
require_once("$lib_path/PhpSpreadsheet/Style/Conditional.php");
require_once("$lib_path/PhpSpreadsheet/Style/Fill.php");
require_once("$lib_path/PhpSpreadsheet/Style/Font.php");
require_once("$lib_path/PhpSpreadsheet/Style/NumberFormat/Formatter.php");
require_once("$lib_path/PhpSpreadsheet/Style/NumberFormat.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/BaseDrawing.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/Drawing.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/MemoryDrawing.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx/WriterPart.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx/AutoFilter.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx/Chart.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx/Comments.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx/ContentTypes.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx/DefinedNames.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx/DocProps.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx/Drawing.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx/Rels.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx/RelsRibbon.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx/RelsVBA.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx/StringTable.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx/Style.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx/Table.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx/Theme.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx/Workbook.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx/Worksheet.php");
require_once("$lib_path/PhpSpreadsheet/Writer/Xlsx.php");

// Reader
require_once("$lib_path/PhpSpreadsheet/Cell/Coordinate.php");
require_once("$lib_path/PhpSpreadsheet/Cell/CellAddress.php");
require_once("$lib_path/PhpSpreadsheet/Cell/DataType.php");
require_once("$lib_path/PhpSpreadsheet/Cell/Hyperlink.php");
require_once("$lib_path/PhpSpreadsheet/Cell/IValueBinder.php");
require_once("$lib_path/PhpSpreadsheet/Cell/DefaultValueBinder.php");
require_once("$lib_path/PhpSpreadsheet/Cell/Cell.php");
require_once("$lib_path/PhpSpreadsheet/DefinedName.php");
require_once("$lib_path/PhpSpreadsheet/RichText/ITextElement.php");
require_once("$lib_path/PhpSpreadsheet/RichText/TextElement.php");
require_once("$lib_path/PhpSpreadsheet/RichText/RichText.php");
require_once("$lib_path/PhpSpreadsheet/RichText/Run.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Security/XmlScanner.php");
require_once("$lib_path/PhpSpreadsheet/Reader/IReader.php");
require_once("$lib_path/PhpSpreadsheet/Reader/BaseReader.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Csv/Delimiter.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Csv.php");
require_once("$lib_path/PhpSpreadsheet/Reader/IReadFilter.php");
require_once("$lib_path/PhpSpreadsheet/Reader/DefaultReadFilter.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Exception.php");

// .xlsx
require_once("$lib_path/PhpSpreadsheet/Reader/Xlsx/AutoFilter.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xlsx/Chart.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xlsx/BaseParserClass.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xlsx/ColumnAndRowAttributes.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xlsx/ConditionalStyles.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xlsx/DataValidations.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xlsx/Hyperlinks.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xlsx/Namespaces.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xlsx/PageSetup.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xlsx/Properties.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xlsx/SheetViewOptions.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xlsx/SheetViews.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xlsx/Styles.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xlsx/TableReader.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xlsx/Theme.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xlsx/WorkbookView.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xlsx.php");

// .xls
require_once("$lib_path/PhpSpreadsheet/Shared/CodePage.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xls/Color/BuiltIn.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xls/Color/BIFF5.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xls/Color/BIFF8.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xls/Color.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xls/ConditionalFormatting.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xls/DataValidationHelper.php");
require_once("$lib_path/PhpSpreadsheet/Shared/Escher/DggContainer.php");
require_once("$lib_path/PhpSpreadsheet/Shared/Escher/DgContainer/SpgrContainer/SpContainer.php");
require_once("$lib_path/PhpSpreadsheet/Shared/Escher/DgContainer/SpgrContainer.php");
require_once("$lib_path/PhpSpreadsheet/Shared/Escher/DgContainer.php");
require_once("$lib_path/PhpSpreadsheet/Shared/Escher.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xls/Escher.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xls/ErrorCode.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xls/RC4.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xls/MD5.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xls/Style/Border.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xls/Style/CellAlignment.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xls/Style/CellFont.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xls/Style/FillPattern.php");
require_once("$lib_path/PhpSpreadsheet/Reader/Xls.php");

require_once("$lib_path/PhpSpreadsheet/ReferenceHelper.php");
require_once("$lib_path/PhpSpreadsheet/RichText/RichText.php");
require_once("$lib_path/PhpSpreadsheet/Settings.php");
require_once("$lib_path/PhpSpreadsheet/Shared/Date.php");
require_once("$lib_path/PhpSpreadsheet/Shared/Drawing.php");
require_once("$lib_path/PhpSpreadsheet/Shared/File.php");
require_once("$lib_path/PhpSpreadsheet/Shared/Font.php");
require_once("$lib_path/PhpSpreadsheet/Shared/StringHelper.php");
require_once("$lib_path/PhpSpreadsheet/Style/Color.php");
require_once("$lib_path/PhpSpreadsheet/Style/Style.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/HeaderFooterDrawing.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/Worksheet.php");
require_once("$lib_path/PhpSpreadsheet/Calculation/Category.php");
require_once("$lib_path/PhpSpreadsheet/Calculation/Engine/CyclicReferenceStack.php");
require_once("$lib_path/PhpSpreadsheet/Calculation/Engine/Logger.php");
require_once("$lib_path/PhpSpreadsheet/Calculation/Engine/BranchPruner.php");
require_once("$lib_path/PhpSpreadsheet/Collection/Memory/SimpleCache3.php");
require_once("$lib_path/PhpSpreadsheet/Collection/Cells.php");
require_once("$lib_path/PhpSpreadsheet/Collection/CellsFactory.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/PageSetup.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/PageMargins.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/HeaderFooter.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/SheetView.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/Protection.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/Dimension.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/RowDimension.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/ColumnDimension.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/Iterator.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/AutoFilter.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/AutoFilter/Column.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/AutoFilter/Column/Rule.php");
require_once("$lib_path/PhpSpreadsheet/Document/Properties.php");
require_once("$lib_path/PhpSpreadsheet/Document/Security.php");
require_once("$lib_path/PhpSpreadsheet/Shared/IntOrFloat.php");
require_once("$lib_path/PhpSpreadsheet/Style/Alignment.php");
require_once("$lib_path/PhpSpreadsheet/Style/Border.php");
require_once("$lib_path/PhpSpreadsheet/Style/Protection.php");
require_once("$lib_path/PhpSpreadsheet/Worksheet/Validations.php");
