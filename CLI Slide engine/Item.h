#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include <memory>
#include <map>
#include <fstream>


class Shape{
public:
    virtual std::pair<std::pair<double, double>, std::pair<double, double>> bounding_box() const = 0;
    virtual ~Shape() = default;
    virtual std::pair<std::pair<double, double>, std::pair<double, double>> backtoshape() const = 0;
    // mnacaci hamar return type-???
};

class Rectangle : public Shape {
public:
    Rectangle(std::pair<double, double> ll, std::pair<double, double> ur);
    std::pair<std::pair<double, double>, std::pair<double, double>> bounding_box() const override;
    std::pair<std::pair<double, double>, std::pair<double, double>> backtoshape() const override;
private:
    std::pair<double, double> m_ll = {0, 0};  
    std::pair<double, double> m_ur = {1, 1};  
};

class Slide{
public:
    void add_shape(std::shared_ptr<Shape> shape); 
    void remove_shape();  
    void remove_shape(int ID);
    std::shared_ptr<Shape> get_shape(int ID);
    int shape_count() const { return m_countShapes; }


private:
    std::vector<std::pair<int, std::shared_ptr<Shape>>> m_shapes; 
    int m_countShapes = 0;  
};

class Project {
public:
    Project();
    void add_slide(int ID);
    Slide& get_slide(int ID);
    void remove_slide(int ID);
    void remove_slide();
    std::ofstream serializeCSV(const std::string& filename) const;
private:
    std::vector<std::pair<int, Slide>> m_slides; 
    int m_countSlide = 1;
};

#endif //ITEM_H