class Bullet {
protected:
	float shootAngle, remainingDistance;
	float currentX, currentY;
public: 
	Bullet(float, float, float);
	~Bullet(); 
	float getCurrentX();
	float getCurrentY();
	float getShootAngle();
	float getRemainingDistance();
	void setX(float);
	void setY(float);
	void setRemainingDistance(float);
		
};