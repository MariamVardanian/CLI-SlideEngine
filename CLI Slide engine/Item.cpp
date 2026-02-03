#include "Item.h"
#include <stdexcept>

Rectangle::Rectangle(std::pair<double, double> ll, std::pair<double, double> ur){
    if(ll.first < ur.first && ll.second < ur.second){
        m_ll = ll;
        m_ur = ur;
    }
    else{
        throw std::runtime_error{
            "Invalid coordinates for rectangle"
        };
    }
}

std::pair<std::pair<double, double>, std::pair<double, double>> Rectangle::bounding_box() const {
    return{{m_ll.first, m_ur.second},{m_ur.second - m_ll.second, m_ur.first - m_ll.first}};
}   

std::pair<std::pair<double, double>, std::pair<double, double>> Rectangle::backtoshape() const {
    return {m_ll, m_ur};
}

void Slide::add_shape(std::shared_ptr<Shape> shape){
    m_shapes.push_back({++m_countShapes, std::move(shape)});
} 

void Slide::remove_shape(){
    m_shapes.pop_back();
    --m_countShapes;
}

void Slide::remove_shape(int ID){
    for(int i = ID; i < m_countShapes; ++i){
        --m_shapes[i].first;
    }    
    m_shapes.erase(m_shapes.begin() + ID - 1);
    --m_countShapes;
}

std::shared_ptr<Shape> Slide::get_shape(int ID){
    return m_shapes[ID-1].second;
}

Project::Project(){
    Slide firstSlide;
    m_slides.push_back({1, firstSlide});
}

Slide& Project::get_slide(int ID){
    if (ID < 1 || ID > m_countSlide) {
        throw std::out_of_range("Invalid slide ID");
    }
    return m_slides[ID-1].second;
}

void Project::add_slide(int ID){
    if(ID <= m_countSlide + 1){
        m_slides.emplace(m_slides.cbegin() + ID - 1, ID, Slide{});
        ++m_countSlide;
    for (int i = ID; i < m_slides.size(); ++i) {
            m_slides[i].first = i + 1;
        }
    }
    else{
        throw std::runtime_error{"There are too few slides"}; 
    }
}

void Project::remove_slide(){
    m_slides.pop_back();
    --m_countSlide;
}

void Project::remove_slide(int ID){
    for(int i = ID; i < m_countSlide; ++i){
        --m_slides[i].first;
    }    
    m_slides.erase(m_slides.begin() + ID - 1);
    --m_countSlide;    
}

std::ofstream Project::serializeCSV(const std::string& filename) const {
    std::ofstream file(filename);
    for(auto elem : m_slides){
        file << "slide," << elem.first << std::endl;
        for(int i = 0; i < elem.second.shape_count(); ++i){
            file << "shape," << i + 1 << "," << elem.second.get_shape(i+1)->backtoshape().first.first << "," 
                 << elem.second.get_shape(i+1)->backtoshape().first.second << ","
                 << elem.second.get_shape(i+1)->backtoshape().second.first << ","
                 << elem.second.get_shape(i+1)->backtoshape().second.second << std::endl;
        }
    }
    return file;
}