const int RENDERER_INITIALIZATION_EXCEPTION = 0;

class Exception {
private:
    int type;
public:
    Exception(const int type) {
        this->type = type;
    }

    int getType() {
        return type;
    }
};