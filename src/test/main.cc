
#include <iostream>
#include <fstream>
#include <string>

#include "src/sharing/algo/sharing_manager.hpp"
#include "dependencies/httplib.h"

using json = nlohmann::json;

void SaveFile(std::string save_path, std::string file_str) {
  std::ofstream writer(save_path);
  writer.clear();
  writer << file_str;
  writer.close();
}
std::string ReadFile(std::string read_path) {
  std::string file_str;
  std::ifstream reader = std::ifstream(read_path);
  std::stringstream strStream;
  if (reader.is_open()) {
    strStream << reader.rdbuf();
  }

  reader.close();
  return strStream.str();
}


// URL编码函数
std::string url_encode(const std::string &value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (char c : value) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            escaped << '%' << std::setw(2) << int((unsigned char) c);
        }
    }

    return escaped.str();
}

// 生成HTML内容
std::string generate_html(const std::string &geojson_io_url) {
    std::ostringstream html;
    html << "<!DOCTYPE html>\n";
    html << "<html>\n";
    html << "<head>\n";
    html << "    <title>GeoJSON.io Link</title>\n";
    html << "</head>\n";
    html << "<body>\n";
    html << "    <p>Click the link below to view your GeoJSON data on GeoJSON.io:</p>\n";
    html << "    <a href=\"" << geojson_io_url << "\">View GeoJSON Data</a>\n";
    html << "</body>\n";
    html << "</html>\n";
    return html.str();
}

int main() {

    // 创建HTTP服务器
    httplib::Server svr;

    svr.Get("/", [&](const httplib::Request &, httplib::Response &res) {
        std::string parameter_str = ReadFile("/mnt/test_data/input/parameter.json");
        json solution_str = airouting::airsharing::Dispatch_to_json(parameter_str);

        // 构建GeoJSON.io的URL
        std::string geojson_io_url = "http://geojson.io/#data=data:application/json," + url_encode(solution_str["geo_json"].dump());

        std::string html_content = generate_html(geojson_io_url);
        res.set_content(html_content, "text/html");
    });

    std::cout << "Server started at http://localhost:5000" << std::endl;
    svr.listen("0.0.0.0", 5000);


  return 0;
}


