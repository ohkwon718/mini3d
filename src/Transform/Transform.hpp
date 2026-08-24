#include <eigen3/Eigen/Dense>

class Transform 
{
    public:
        Transform();
               
        Eigen::Vector3f translation() const;
        Eigen::Quaternionf rotation() const;
        Eigen::Vector3f scale() const;

        void setTranslation(const Eigen::Vector3f& translation);
        void setRotation(const Eigen::Quaternionf& rotation);
        void setScale(const Eigen::Vector3f& scale);

        Eigen::Matrix4f matrix() const;
    private:    
        Eigen::Vector3f translation_;
        Eigen::Quaternionf rotation_;
        Eigen::Vector3f scale_;
};


