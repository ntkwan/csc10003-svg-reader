#ifndef GROUP_HPP_
#define GROUP_HPP_

#include <vector>

#include "SVGElement.hpp"

typedef std::vector< std::pair< std::string, std::string > > Attributes;

class Group : public SVGElement {
public:
    Group();

    Group(Attributes attributes);

    ~Group();

    std::string getClass() const override;

    Attributes getAttributes() const;

    void addElement(SVGElement* shape) override;

    std::vector< SVGElement* > getElements() const;

    void printData() const override;

private:
    std::vector< SVGElement* > shapes;
    Attributes attributes;
};

#endif  // GROUP_HPP_