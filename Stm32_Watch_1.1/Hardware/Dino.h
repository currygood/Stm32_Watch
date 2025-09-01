#ifndef __DINO_H
#define __DINO_H

typedef struct 
{
	uint8_t minX,minY,maxX,maxY;
}Object_Position;		//物体四个角坐标，其实保存xy和最小xy就行了

void Show_Score(void);
void DinoGame_Animation(void);
void Show_Ground(void);
void Dino_Tick(void);
void Show_Barrier(void);

#endif
