#pragma once

#include "Node.h"

#include <glm/glm.hpp>
#include <memory>

class Table;
using TablePtr = std::shared_ptr<Table>;

class Table : public Node
{   

    public :  

        class Leg : public Node
        {
            public: 
                using LegPtr = std::shared_ptr<Leg>;

                static LegPtr Make(const glm::vec3& pos, float height);
                Leg(const glm::vec3& pos, float height);
                ~Leg();
        };

        Table(const glm::vec3& pos);
        void setupLegs();

        ~Table();

    
        static TablePtr Make(const glm::vec3& pos);
    
};