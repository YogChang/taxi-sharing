

#ifndef GEOHASH_JSON_H_
#define GEOHASH_JSON_H_

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

#ifndef NLOHMANN_JSON_H_
#include "nlohmann/json.hpp"
#define NLOHMANN_JSON_H_
#endif

namespace geo_hash_json
{

#define BASE32 "0123456789bcdefghjkmnpqrstuvwxyz"
  static const int UNBASE32[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15, 16, 0, 17, 18, 0, 19, 20, 0,
                                 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
  static const char CharDic16[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
                                   '9', 'A', 'B', 'C', 'D', 'E', 'F'};


  class Node
  {
  private:
    std::string geo_hash_id = "xn1ruxsmu";
    std::pair<double, double> gps_coordinate = {0, 0};
    std::vector<std::pair<std::string, std::string>> name_value_properties = {};
    void GeoHashDecode();
    void GeoHashEncode();
    void ToColor(const std::string &input_color);
    std::pair<double, double> GetGPS();
    nlohmann::json ToJson();
    
    friend class Vehicle;

  public:

    explicit Node(const std::string &input_geo_hash_id);
    explicit Node(const double &input_lng, const double &input_lat);
    void AddProperty(const std::string &property_name, const std::string &property_value);
  };
  Node::Node(const std::string &input_geo_hash_id)
  {
    if (input_geo_hash_id.compare("") == 0)
    {
      this->AddProperty("error", "empty string");
      this->AddProperty("arrival reason", "commemorate Rex");
    } else
    {
      this->geo_hash_id = input_geo_hash_id;
    }

    this->GeoHashDecode();
    // this->AddProperty("location", this->geo_hash_id);
  }
  Node::Node(const double &input_lng, const double &input_lat)
  {
    this->gps_coordinate = {input_lng, input_lat};
    this->GeoHashEncode();
    // this->AddProperty("location", this->geo_hash_id);
  }
  void Node::ToColor(const std::string &input_color)
  {
    this->AddProperty("marker-color", input_color);
  }
  std::pair<double, double> Node::GetGPS()
  {
    return this->gps_coordinate;
  }
  void Node::AddProperty(const std::string &property_name, const std::string &property_value)
  {
    std::pair<std::string, std::string> temp_pair = {property_name, property_value};
    this->name_value_properties.push_back(temp_pair);
  }
  nlohmann::json Node::ToJson()
  {
    nlohmann::json json_properties;
    for (auto &ele : this->name_value_properties)
    {
      json_properties[ele.first] = ele.second;
    }
    nlohmann::json geometry = {
        {"type", "Point"},
        {"coordinates", {this->gps_coordinate.first, this->gps_coordinate.second}},
    };
    nlohmann::json node_json = {
        {"properties", json_properties},
        {"geometry", geometry},
        {"type", "Feature"}};

    return node_json;
  }
  void Node::GeoHashDecode()
  {
    char is_even = 1;
    char index = 0;
    char tmp1 = 0;
    int bit = 0;
    double lats[2];
    double lons[2];
    double mid;
    char *p = &(this->geo_hash_id.at(0));

    int value_precision = 0;
    int *precision = &value_precision;
    lats[0] = -90.0;
    lats[1] = 90.0;
    lons[0] = -180.0;
    lons[1] = 180.0;

    while (0 != *p)
    {
      bit = 0;
      index = UNBASE32[(*p - '0')];

      while (bit < 5)
      {
        tmp1 = (index >> (4 - bit)) & 0x01;
        if (is_even)
        {
          mid = (lons[0] + lons[1]) / 2;
          if (tmp1)
            lons[0] = mid;
          else
            lons[1] = mid;
        }
        else
        {
          mid = (lats[0] + lats[1]) / 2;
          if (tmp1)
            lats[0] = mid;
          else
            lats[1] = mid;
        }
        is_even = !is_even;
        bit++;
      }
      (*precision)++;
      p++;
    }
    this->gps_coordinate = {(lons[0] + lons[1]) / 2, (lats[0] + lats[1]) / 2};
  }
  void Node::GeoHashEncode()
  {
    this->geo_hash_id.clear();
    int precision = 9;
    char is_odd = 0;
    int i = 0;
    double lats[2];
    double lons[2];
    double mid;
    char bits[] = {0x10, 0x08, 0x04, 0x02, 0x01};
    int bit = 0;
    char tmp = 0;

    lats[0] = -90.0;
    lats[1] = 90.0;
    lons[0] = -180.0;
    lons[1] = 180.0;

    while (i < precision)
    {
      if (is_odd)
      {
        mid = (lats[0] + lats[1]) / 2;
        if (this->gps_coordinate.second > mid)
        {
          tmp |= bits[bit];
          lats[0] = mid;
        }
        else
        {
          lats[1] = mid;
        }
      }
      else
      {
        mid = (lons[0] + lons[1]) / 2;
        if (this->gps_coordinate.first > mid)
        {
          tmp |= bits[bit];
          lons[0] = mid;
        }
        else
        {
          lons[1] = mid;
        }
      }
      is_odd = !is_odd;
      if (bit < 4)
      {
        bit++;
      }
      else
      {
        this->geo_hash_id.push_back(BASE32[static_cast<int>(tmp)]);
        i++;
        bit = 0;
        tmp = 0;
      }
    }
  }

  class Vehicle
  {
  private:
    std::string vehicle_id = "";
    std::string line_color = "#000000";
    std::vector<Node> routes = {};
    void ToColor(const std::string &input_color);
    nlohmann::json LineJson(const bool &is_color);
    nlohmann::json ToJson();

  public:
    explicit Vehicle(const std::string &input_vehicle_id);
    void AddNode(Node input_node);

    friend class GeoHashJson;
  };
  Vehicle::Vehicle(const std::string &input_vehicle_id)
  {
    this->vehicle_id = input_vehicle_id;
  }
  void Vehicle::ToColor(const std::string &input_color)
  {
    this->line_color = input_color;
    for (auto &ele : this->routes)
    {
      ele.ToColor(input_color);
    }
  }
  void Vehicle::AddNode(Node input_node)
  {
    input_node.AddProperty("車輛代碼", this->vehicle_id);
    input_node.AddProperty("抵達次序", std::to_string(this->routes.size()));
    this->routes.push_back(input_node);
  }
  nlohmann::json Vehicle::LineJson(const bool &is_color)
  {
    std::string temp_line_color = "#000000";
    int temp_line_width = 1;
    if (is_color)
    {
      temp_line_color = this->line_color;
      temp_line_width = 5;
    }

    std::vector<nlohmann::json> line_coordinates;
    for (auto &ele : this->routes)
    {
      line_coordinates.push_back(ele.GetGPS());
    }

    nlohmann::json line_json = {
        {"geometry", {{"type", "LineString"}, {"coordinates", line_coordinates}}},
        {"properties", {{"stroke", temp_line_color}, {"stroke-opacity", 1}, {"stroke-width", temp_line_width}}},
        {"type", "Feature"}};
    return line_json;
  }
  nlohmann::json Vehicle::ToJson()
  {
    std::vector<nlohmann::json> vehicle_json;

    for (auto &ele : this->routes)
    {
      nlohmann::json node_json = ele.ToJson();
      vehicle_json.push_back(node_json);
    }

    if (this->routes.size() < 2)
    {
      return vehicle_json;
    }
    vehicle_json.push_back(this->LineJson(true));
    vehicle_json.push_back(this->LineJson(false));

    return vehicle_json;
  }

  class GeoHashJson
  {
  private:
    std::vector<Vehicle> geo_hash_json;
    void ToColorVehicle();

  public:
    void AddVehicle(const Vehicle &input_vehicle);
    nlohmann::json ToJson();
  };
  void GeoHashJson::ToColorVehicle()
  {
    int num_of_vehicle = this->geo_hash_json.size();
    std::vector<std::string> vehicle_colors;

    for (int i = 0; i < num_of_vehicle; i++)
    {
      double hue = (i * 360) / num_of_vehicle;
      double saturation = 0.5;
      double value = 1;

      int hue_region = (hue / 60);
      double f = hue / 60 - hue_region;

      double p = value * (1 - saturation);
      double q = value * (1 - f * saturation);
      double t = value * (1 - (1 - f) * saturation);

      std::vector<double> color;
      if (hue_region == 0)
      {
        color = {value * 255, t * 255, p * 255};
      }
      else if (hue_region == 1)
      {
        color = {q * 255, value * 255, p * 255};
      }
      else if (hue_region == 2)
      {
        color = {p * 255, value * 255, t * 255};
      }
      else if (hue_region == 3)
      {
        color = {p * 255, q * 255, value * 255};
      }
      else if (hue_region == 4)
      {
        color = {t * 255, p * 255, value * 255};
      }
      else if (hue_region == 5)
      {
        color = {value * 255, p * 255, q * 255};
      }

      std::string temp_str = "#";
      for (auto &ele : color)
      {
        int int_num = static_cast<int>(ele);
        int remainder_num = int_num % 16;
        int quotient_num = (int_num - remainder_num) / 16;
        remainder_num = int_num - quotient_num * 16;
        temp_str.append({CharDic16[quotient_num], CharDic16[remainder_num]});
      }

      vehicle_colors.push_back(temp_str);
    }

    for (size_t i = 0; i < this->geo_hash_json.size(); i++)
    {
      Vehicle *ptr_vehicle_i = &(this->geo_hash_json.at(i));
      (*ptr_vehicle_i).ToColor(vehicle_colors.at(i));
    }
  }
  void GeoHashJson::AddVehicle(const Vehicle &input_vehicle)
  {
    this->geo_hash_json.push_back(input_vehicle);
  };
  nlohmann::json GeoHashJson::ToJson()
  {
    this->ToColorVehicle();

    std::vector<nlohmann::json> features;

    for (auto &veh : this->geo_hash_json)
    {
      for (auto &ele : veh.ToJson())
      {
        features.push_back(ele);
      }
    }
    nlohmann::json result = {
        {"type", "FeatureCollection"},
        {"features", features}};

    return result;
  }

} // namespace geo_hash_json

#endif // GEOHASH_JSON_H_
