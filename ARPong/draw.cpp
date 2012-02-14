#include "ARPong.h"
#include "glm.h"

const float BALL_Z = 10.0f;
const float BALL_SPEED = 20.0f; /* Actually speed/sqrt(2) */
const float BALL_RADIUS = 15.0f;

float ball_x = 100.0f, ball_y = -50.0f;
float ball_vx = BALL_SPEED, ball_vy = BALL_SPEED;

GLMmodel *field;
GLMmodel *pad;

void draw_reset(void)
{
	ball_x = 100.0f; ball_y = -50.0f;
	ball_vx = BALL_SPEED; ball_vy = BALL_SPEED;
}

void draw_init(void)
{
	/* Display list for the field */
	field = glmReadOBJ("Data/field.obj");
	if(field == NULL)
	{
		printf("Unable to load models\n");
		exit(1);
	}

	/* Display list for the pad */
	pad = glmReadOBJ("Data/pad.obj");
	if(pad == NULL)
	{
		printf("Unable to load models\n");
		exit(1);
	}
}

void reflectOnPad(ARMat *mat_field, double pad_trans[3][4])
{
	int i, j;
	/*
	* RAQ * BALL_LOCAL = FIELD * BALL_ABS
	* BALL_LOCAL = RAQ-1 * FIELD * BALL_ABS
	*/
	ARMat *mat_raq = arMatrixAlloc(4, 4);
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 4; j++)
			mat_raq->m[i*4 + j] = pad_trans[i][j];
	}
	mat_raq->m[3*4 + 0] = 0.0;
	mat_raq->m[3*4 + 1] = 0.0;
	mat_raq->m[3*4 + 2] = 0.0;
	mat_raq->m[3*4 + 3] = 1.0;
	arMatrixSelfInv(mat_raq);

	ARMat *field2pad = arMatrixAlloc(4, 4);
	arMatrixMul(field2pad, mat_raq, mat_field);

	float ball_lx = field2pad->m[0*4 + 0] * ball_x + field2pad->m[0*4 + 1] * ball_y + field2pad->m[0*4 + 2] * BALL_Z + field2pad->m[0*4 + 3];
	float ball_ly = field2pad->m[1*4 + 0] * ball_x + field2pad->m[1*4 + 1] * ball_y + field2pad->m[1*4 + 2] * BALL_Z + field2pad->m[1*4 + 3];
	float ball_lz = field2pad->m[2*4 + 0] * ball_x + field2pad->m[2*4 + 1] * ball_y + field2pad->m[2*4 + 2] * BALL_Z + field2pad->m[2*4 + 3];

	printf("Position: (%lf, %lf, %lf)\n", ball_lx, ball_ly, ball_lz);

	if(ball_lx >= -60.0f - BALL_RADIUS && ball_lx <= -60.0f + BALL_RADIUS
	 && ball_ly >= -45.0f - BALL_RADIUS && ball_ly <= 45.0f + BALL_RADIUS
	 && ball_lz >= -45.0f - BALL_RADIUS && ball_lz <= 45.0f + BALL_RADIUS)
	{
		printf("Collision!\n");
		/* The normal to the pad in field space */
		ARMat *pad2field = arMatrixAlloc(4, 4);
		arMatrixInv(pad2field, field2pad);

		float normal_x = pad2field->m[0*4 + 0];
		float normal_y = pad2field->m[1*4 + 0];
		float normal_z = pad2field->m[2*4 + 0];

		float dotprod = normal_x * ball_vx + normal_y * ball_vy;
		if(dotprod >= 0.0f)
		{
			printf("Bounce!\n");
			/* Bounce! */
			ball_vx -= 2.0f * dotprod * normal_x;
			ball_vy -= 2.0f * dotprod * normal_y;
		}

		arMatrixFree(pad2field);
	}


	arMatrixFree(field2pad);
	arMatrixFree(mat_raq);
}

void draw(bool field_visible, double field_trans[3][4], bool pad1_visible, double pad1_trans[3][4])
{
	double gl_para[16];
	GLfloat mat_ambient_red[] = {1.0f, 0.0f, 0.0f, 1.0f};
	GLfloat mat_flash_red[] = {1.0f, 0.0f, 0.0f, 1.0f};
	GLfloat mat_flash_shiny[] = {50.0f};
	GLfloat light_position[] = {100.0f, -50.0f, 0.0f, 0.0f};
	GLfloat ambi[] = {0.1f, 0.1f, 0.1f, 0.1f};
	GLfloat lightZeroColor[] = {0.9f, 0.9f, 0.9f, 0.1f};

	argDrawMode3D();
	argDraw3dCamera(0, 0);
	glClearDepth(1.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_LIGHTING);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);

	if(field_visible)
	{
		/* Load the camera transformation matrix for the field */
		argConvGlpara(field_trans, gl_para);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(gl_para);

		/* Draw the field */
		glPushMatrix();
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glmDraw(field, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
		glPopMatrix();

		/* Move the ball */
		float old_ball_x = ball_x;
		float old_ball_y = ball_y;
		float elapsed = timeElapsed();
		ball_x += ball_vx * elapsed;
		ball_y += ball_vy * elapsed;

		/* Draw the ball */
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_flash_red);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_flash_shiny);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient_red);

		glPushMatrix();
		glTranslatef(ball_x, ball_y, BALL_Z);
		glutSolidSphere(BALL_RADIUS, 16.0f, 16.0f);
		glPopMatrix();

		ARMat *mat_field = NULL;
		if(pad1_visible /* || pad2_visible*/)
		{
			int i, j;
			mat_field = arMatrixAlloc(4, 4);
			for(i = 0; i < 3; i++)
			{
				for(j = 0; j < 4; j++)
					mat_field->m[i*4 + j] = field_trans[i][j];
			}
			mat_field->m[3*4 + 0] = 0.0;
			mat_field->m[3*4 + 1] = 0.0;
			mat_field->m[3*4 + 2] = 0.0;
			mat_field->m[3*4 + 3] = 1.0;
		}

		if(pad1_visible)
		{
			/* Load the camera transformation matrix for pad1 */
			argConvGlpara(pad1_trans, gl_para);
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixd(gl_para);

			glPushMatrix();
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			glmDraw(pad, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
			glPopMatrix();

			reflectOnPad(mat_field, pad1_trans);
		}

		if(mat_field != NULL)
			arMatrixFree(mat_field);
	}

	glDisable(GL_LIGHT0);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
}
