#ifndef CELL_HPP
#define CELL_HPP

#include <iostream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <random>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

class CellParams {
    //The CellParams class stores the parameters of a particular cell.
public:
    std::string name;
};

class CellConfig{
    // Abstract base class for cell configurations.
public:
    virtual ~CellConfig() = default;
};

class PerturbParams {
    //Used with a CellConfig to add perturb parameters.
public:
    float prob;
    float mu;
    float sigma;

    float get_perturb_offset() const {
        if (rand() / static_cast<float>(RAND_MAX) < prob) {
            return mu + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * sigma;
        }
        else {
            return mu;
        }
    }
    //void parseParams(const YAML::Node& node) {
    //    prob = node["prob"].as<double>();
    //    mu = node["mu"].as<double>();
    //    sigma = node["sigma"].as<double>();
    //}
};

class SimulationConfig; //just for declaration in draw function

class Cell {
    //The Cell class stores information about a particular cell.
protected:
    CellParams paramClass;
    CellConfig cellConfig;
public:
    Cell(const CellParams& initProps) : paramClass(initProps), cellConfig() {}
    Cell() {}
    virtual ~Cell() = default;

    virtual void draw(cv::Mat& image, SimulationConfig simulationConfig, cv::Mat* cellMap = nullptr, float z = 0) const = 0;
    virtual void draw_outline(cv::Mat& image, cv::Scalar color, float z = 0) const = 0;
    virtual Cell* get_perturbed_cell() const = 0;
    virtual Cell* get_parameterized_cell(std::unordered_map<std::string, float> params = {}) const = 0;
    virtual std::tuple<Cell*, Cell*, bool> get_split_cells() const = 0;
    virtual CellParams get_cell_params() const = 0;
    static bool check_if_cells_valid(const std::vector<Cell*>& cells) { return false; }
    virtual std::pair<std::vector<float>, std::vector<float>> calculate_corners() const = 0;
    virtual std::pair<std::vector<float>, std::vector<float>> calculate_minimum_box(Cell& perturbed_cell) const = 0;
};

#endif
