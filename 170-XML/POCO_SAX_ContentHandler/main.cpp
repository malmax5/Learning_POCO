#include "Poco/SAX/ContentHandler.h"
#include "Poco/SAX/Attributes.h"
#include "Poco/SAX/SAXParser.h"
#include "Poco/XML/XMLString.h"
#include <iostream>
#include <fstream>
#include <stack>

class SimpleContentHandler : public Poco::XML::ContentHandler
{
private:
    std::stack<std::string> elementStack;
    int indentLevel = 0;
    
    void printIndent()
    {
        for (int i = 0; i < indentLevel; ++i)
        {
            std::cout << "  ";
        }
    }

public:
    void startDocument() override
    {
        std::cout << "=== Start Document ===" << std::endl;
    }

    void endDocument() override
    {
        std::cout << "=== End Document ===" << std::endl;
    }

    void startElement(const Poco::XML::XMLString& uri,
                     const Poco::XML::XMLString& localName,
                     const Poco::XML::XMLString& qname,
                     const Poco::XML::Attributes& attributes) override
    {
        printIndent();
        std::cout << "Start: " << qname << std::endl;
        
        // Обработка атрибутов
        for (int i = 0; i < attributes.getLength(); ++i)
        {
            printIndent();
            std::cout << "  Attr: " << attributes.getQName(i) 
                      << " = '" << attributes.getValue(i) << "'" << std::endl;
        }
        
        elementStack.push(qname);
        indentLevel++;
    }

    void endElement(const Poco::XML::XMLString& uri,
                   const Poco::XML::XMLString& localName,
                   const Poco::XML::XMLString& qname) override
    {
        indentLevel--;
        printIndent();
        std::cout << "End: " << qname << std::endl;
        
        if (!elementStack.empty())
        {
            elementStack.pop();
        }
    }

    void characters(const Poco::XML::XMLChar ch[], 
                   int start, int length) override
    {
        std::string text(ch + start, length);
        // Убираем пробельные символы
        if (!text.empty() && text.find_first_not_of(" \t\n\r") != std::string::npos)
        {
            printIndent();
            std::cout << "Text: '" << text << "'" << std::endl;
        }
    }

    void ignorableWhitespace(const Poco::XML::XMLChar ch[], 
                            int start, int length) override
    {
        // Игнорируем пробельные символы
    }

    void processingInstruction(const Poco::XML::XMLString& target,
                              const Poco::XML::XMLString& data) override
    {
        printIndent();
        std::cout << "PI: " << target << " -> " << data << std::endl;
    }

    void setDocumentLocator(const Poco::XML::Locator* /*loc*/) override
    {
        // Обычно не используется в простых парсерах
    }

    void startPrefixMapping(
        const Poco::XML::XMLString& /*prefix*/,
        const Poco::XML::XMLString& /*uri*/) override
    {
        // Игнорируем пространства имён, если не нужны
    }

    void endPrefixMapping(const Poco::XML::XMLString& /*prefix*/) override
    {
        // Игнорируем
    }

    void skippedEntity(const Poco::XML::XMLString& /*name*/) override
    {
        // Игнорируем пропущенные сущности
    }
};

void sax_parsing_demo()
{
    std::cout << "=== SAX Parsing Demo ===" << std::endl;
    
    // Создаем тестовый XML файл
    const char* test_xml = R"(<?xml version="1.0" encoding="UTF-8"?>
<!-- This is a test document -->
<library>
    <book id="1" category="fiction">
        <title>The Great Gatsby</title>
        <author>F. Scott Fitzgerald</author>
        <year>1925</year>
        <price currency="USD">12.99</price>
    </book>
    <book id="2" category="science">
        <title>A Brief History of Time</title>
        <author>Stephen Hawking</author>
        <year>1988</year>
        <price currency="USD">15.50</price>
    </book>
    <?display table-view?>
</library>)";
    
    // Сохраняем во временный файл
    std::ofstream out("test_library.xml");
    out << test_xml;
    out.close();
    
    try
    {
        SimpleContentHandler handler;
        Poco::XML::SAXParser parser;
        parser.setContentHandler(&handler);
        parser.parse("test_library.xml");
        
        std::cout << "SAX parsing completed successfully!" << std::endl;
    }
    catch (Poco::Exception& e)
    {
        std::cout << "Error: " << e.displayText() << std::endl;
    }
}

int main()
{
    sax_parsing_demo();
}
