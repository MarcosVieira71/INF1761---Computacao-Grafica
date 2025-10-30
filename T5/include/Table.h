#pragma once

#include "Node.h"

#include <glm/glm.hpp>
#include <memory>

class Table;
using TablePtr = std::shared_ptr<Table>;

class Appearance;
using AppearancePtr = std::shared_ptr<Appearance>;

class Base;
using BasePtr = std::shared_ptr<Base>;


class Table : public Node
{   
    private:
        glm::vec3 _pos;


    public :  

        class Leg : public Node
        {
            public: 
                using LegPtr = std::shared_ptr<Leg>;

                static LegPtr Make(const glm::vec3& pos, float height, ShapePtr s);
                Leg(const glm::vec3& pos, float height, ShapePtr s);
                ~Leg();
        };

        class Top : public Node
        {
            public: 
                using TopPtr = std::shared_ptr<Top>;

                static TopPtr Make(const glm::vec3& scale);
                Top(const glm::vec3& scale);
                ~Top();
        };


        Table(const glm::vec3& pos, AppearancePtr app);
        void setup(BasePtr base);

        ~Table();

    
        static TablePtr Make(const glm::vec3& pos, AppearancePtr app, AppearancePtr legs = {});
    
};