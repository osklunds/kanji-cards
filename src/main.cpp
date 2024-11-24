
#include <iostream>
#include "pugixml.hpp"

// int main() {
//     std::cout << "Hello World!";
//     return 0;
// }

// int main()
// {
//     pugi::xml_document doc;
//     pugi::xml_parse_result result = doc.load_file("sample.xml");
//     std::cout << result << std::endl;

//     pugi::xml_node note = doc.child("note");
//     std::cout << note.name() << std::endl;

//     pugi::xml_node child = note.child("some_tag");
//     std::cout << child.name() << std::endl;

//     pugi::xml_text text = child.text();
//     std::cout << text.get() << std::endl;

//     // for (pugi::xml_node tool: doc.child("Profile").child("Tools").children("Tool")) {
//     //     int timeout = tool.attribute("Timeout").as_int();
        
//     //     if (timeout > 0)
//     //         std::cout << "Tool " << tool.attribute("Filename").value() << " has timeout " << timeout << "\n";
//     // }

//     return 0;
// }
