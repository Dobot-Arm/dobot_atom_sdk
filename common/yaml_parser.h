// Copyright 2023 Unitree. All rights reserved.
#ifndef YAML_PARSER_
#define YAML_PARSER_

#include <yaml-cpp/yaml.h>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <string>

class YamlParser
{
public:
    explicit YamlParser(const std::string &yaml_file) : yaml_file_(yaml_file) { node_ = YAML::LoadFile(yaml_file); }

    float ReadFloatFromYaml(const std::string &node_name) { return node_[node_name].as<float>(); }

    int ReadIntFromYaml(const std::string &node_name) { return node_[node_name].as<int>(); }

    bool ReadBoolFromYaml(const std::string &node_name) { return node_[node_name].as<bool>(); }

    std::string ReadStringFromYaml(const std::string &node_name) { return node_[node_name].as<std::string>(); }

    Eigen::VectorXf ReadVectorFromYaml(const std::string &node_name, int rows)
    {
        Eigen::VectorXf vec(rows);
        for (size_t i = 0; i < node_[node_name].size(); ++i) { vec(i) = node_[node_name][i].as<float>(); }
        return vec;
    }

    Eigen::VectorXf ReadVectorFromYaml(const std::string &node_name)
    {
        Eigen::VectorXf vec(node_[node_name].size());
        for (size_t i = 0; i < node_[node_name].size(); ++i) { vec(i) = node_[node_name][i].as<float>(); }
        return vec;
    }

    Eigen::ArrayXf ReadArrayVectorFromYaml(const std::string &node_name, int rows)
    {
        Eigen::ArrayXf vec(rows);
        for (size_t i = 0; i < node_[node_name].size(); ++i) { vec(i) = node_[node_name][i].as<float>(); }
        return vec;
    }

    Eigen::MatrixXf ReadMatrixFromYaml(const std::string &node_name, int rows, int cols)
    {
        Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> mat(rows, cols);
        int count = 0;
        for (YAML::const_iterator it = node_[node_name].begin(); it != node_[node_name].end(); ++it) {
            for (size_t j = 0; j < it->size(); ++j) {
                mat(count) = (*it)[j].as<float>();
                count++;
            }
        }
        return mat;
    }

    Eigen::ArrayXXf ReadArrayMatrixFromYaml(const std::string &node_name, int rows, int cols)
    {
        Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> mat(rows, cols);
        int count = 0;
        for (YAML::const_iterator it = node_[node_name].begin(); it != node_[node_name].end(); ++it) {
            for (size_t j = 0; j < it->size(); ++j) {
                mat(count) = (*it)[j].as<float>();
                count++;
            }
        }
        return mat;
    }

private:
    std::string yaml_file_;
    YAML::Node node_;
};

#endif  // YAML_PARSER_
