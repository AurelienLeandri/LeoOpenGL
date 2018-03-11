#include <string>

class Material {
  public:
    Material();
    virtual const char *build();
  protected:
    bool _has_changed;
    std::string content;
};
