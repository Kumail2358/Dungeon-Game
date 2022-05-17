#include "dungeon.hpp"
#include<time.h>

string gameState;
Block blocks[xGrid][yGrid];
Player player = {1, 1, 60, 10, {16, 287, 92, 130}, {50, 50, 50, 50}};

//This function can be called to set an element in every block. It's complete, don't modify it...
void setBlock(int row, int col, int shape, int life, int health){
	blocks[row][col].shape = shape;
	blocks[row][col].life = life;
	blocks[row][col].health = health;
	blocks[row][col].mover = {row*yJump, col*xJump, xJump, yJump};
	switch (shape)
	{
	case ROCK:
		blocks[row][col].src = {151, 287, 147, 138};
		break;
	case CORONA:
		blocks[row][col].src = {0,0, 123, 122};
		break;
	case FIRE:
		blocks[row][col].src = {11, 134, 105, 109};
		break;
	case BURGER:
		blocks[row][col].src = {145, 9, 112, 94};
		break;
	case WELL:
		blocks[row][col].src = {154, 164, 102, 66};
		break;	
	default:
		blocks[row][col].src = {7, 443, 127, 114};
		break;
	}

}

//This function is drawing all the objects. It's complete, don't modify it.
void update(SDL_Renderer* gRenderer, SDL_Texture* assets, string direction ){
	SDL_RenderClear( gRenderer );
	for(int row=0;row<xGrid;row++){
		for (int col = 0; col < yGrid; col++)
		{
			SDL_RenderCopy( gRenderer, assets, &blocks[row][col].src, &blocks[row][col].mover );
		}		
	}


	if(direction!="")//if default argument is used.
		makeMove(direction);
	player.mover = {player.col*yJump, player.row*xJump, xJump, yJump};
	SDL_RenderCopy( gRenderer, assets, &player.src, &player.mover );
	// display life
	int r=0, c=0;
	for(int i=0;i<player.life;i++){
		if(i%10==0){
			r=0; c++;
		}
		SDL_Rect mover = {600 + r++*12, 100 + c*20, 10, 15};	
		SDL_RenderCopy( gRenderer, assets, &player.src, &mover );
	}

	for(int i=0 ;i<player.health;i++){
		SDL_Rect mover = {600+i*12, 300, 10, 10};	
		SDL_Rect src = {175, 461, 102, 90};	
		SDL_RenderCopy( gRenderer, assets, &src, &mover );		
	}
	SDL_Rect mover, src;
	if (gameState=="Running"){
		src = {367, 23, 281, 75};
		mover = {600, 400, 100, 50};
		SDL_RenderCopy(gRenderer, assets, &src, &mover);
	}
	else if (gameState=="Lost")
	{
		src = {372, 224, 182, 45};
		mover = {600, 400, 100, 50};
		SDL_RenderCopy(gRenderer, assets, &src, &mover);
	}
	else if (gameState=="Won")
	{
		src = {366, 125, 186, 45};
		mover = {600, 400, 100, 50};
		SDL_RenderCopy(gRenderer, assets, &src, &mover);	
	}
		
	SDL_RenderPresent( gRenderer );	
	SDL_Delay(5);	
}

	// fill this function to create dungeon. 
void createDungeon(){
	// To fill a block, call the setBlock function, the example is given here:
	// Rocks
	for (int i; i < 10; i++){
		setBlock(i, 0, ROCK, -2, 0);
		setBlock(0, i, ROCK, -2, 0);
		setBlock(9, i, ROCK, -2, 0);
		setBlock(i, 9, ROCK, -2, 0);
	}
	for (int i = 1; i < 9; i++){
		for (int j = 1; j < 9; j++){
			if ((i!=1 || j!=1) && (i!=8 || j!=8)){
				int x = rand() % 100 + 1;
				if (x <= 20 && x > 0)
					setBlock(i, j, 1, -20, -2);
				else if (x <= 40 && x > 20)
					setBlock(i, j, 3, -5, 2);
				else if (x <= 70 && x > 40)
					setBlock(i, j, 2, 5, 0);
				else if (x <= 100 && x > 70)
					setBlock(i, j, 4, -10, -4);
			}
		}
	}
	setBlock(8, 8, GOLD, 0, 0);
	// It Sets a fire block at row=3, and column=4, it reduces life by 4, and increases health by 5
	// Other possible values for shape are: ROCK, CORONA, BURGER, WELL, FIRE, GOLD 
}


// This is a demo implementation of makeMove function
// It doesn't follow the game rules at all
// You have to implement it according to game logic
void outcome(int shape){
	if (shape == CORONA){
		player.life -= 20;
		player.health -= 2;
	}
	else if (shape == BURGER){
		player.life += 5;
	}
	else if (shape == WELL){
		player.life -= 5;
		player.health += 2;
	}
	else if (shape == FIRE){
		player.life -= 10;
		player.health -= 4;
	}
	else if (shape == ROCK){
		player.life -= 2;
	}
}
void makeMove(string direction){
	if (gameState == "Won")
	{cout << "You win" << endl;}
	else if (gameState == "Lost")
	{cout << "You lose" << endl;}
	else{
		gameState = "Running"; //Other values are: Lost, Won
		// manipulate the player.life and player.health variable accordingly
		//When Right Arrow is pressed, other possible directions are: left, up, down
		if (direction == "right"){
			// Checks for each kind of block.
			if (blocks[player.col + 1][player.row].shape == CORONA){
				outcome(CORONA);
				player.col++;
			}
			else if (blocks[player.col + 1][player.row].shape == BURGER){
				outcome(BURGER);
				player.col++;
			}
			else if (blocks[player.col + 1][player.row].shape == WELL){
				outcome(WELL);
				player.col++;
			}
			else if (blocks[player.col + 1][player.row].shape == FIRE){
				outcome(FIRE);
				player.col++;
			}
			else if (blocks[player.col + 1][player.row].shape == ROCK){
				outcome(ROCK);
			}
			else if (blocks[player.col + 1][player.row].shape == GOLD){
				gameState = "Won";
				player.col++;
			}
			// Checks for if the player has less than 0 health
			if (player.life <= 0 || player.health <= 0){
				gameState = "Lost";
			}
		}
		else if (direction == "left"){
			// Checks for each kind of block.
			if (blocks[player.col - 1][player.row].shape == CORONA){
				outcome(CORONA);
				player.col--;
			}
			else if (blocks[player.col - 1][player.row].shape == BURGER){
				outcome(BURGER);
				player.col--;
			}
			else if (blocks[player.col - 1][player.row].shape == WELL){
				outcome(WELL);
				player.col--;
			}
			else if (blocks[player.col - 1][player.row].shape == FIRE){
				outcome(FIRE);
				player.col--;
			}
			else if (blocks[player.col - 1][player.row].shape == ROCK){
				outcome(ROCK);
			}
			else if (blocks[player.col - 1][player.row].shape == GOLD){
				player.col--;
				gameState = "Won";
			}
			// Checks for if the player has less than 0 health
			if (player.life <= 0 || player.health <= 0){
				gameState = "Lost";
			}
		}
		else if (direction == "up"){
			// Checks for each kind of block.
			if (blocks[player.col][player.row - 1].shape == CORONA){
				outcome(CORONA);
				player.row--;
			}
			else if (blocks[player.col][player.row - 1].shape == BURGER){
				outcome(BURGER);
				player.row--;
			}
			else if (blocks[player.col][player.row - 1].shape == WELL){
				outcome(WELL);
				player.row--;
			}
			else if (blocks[player.col][player.row - 1].shape == FIRE){
				outcome(FIRE);
				player.row--;
			}
			else if (blocks[player.col][player.row].shape == ROCK){
				outcome(ROCK);
			}
			else if (blocks[player.col][player.row - 1].shape == GOLD){
				player.row--;
				gameState = "Won";
			}
			// Checks for if the player has less than 0 health
			if (player.life <= 0 || player.health <= 0){
				gameState = "Lost";
			}
			//player.row--;
		}
		else if (direction == "down"){
			// Checks for each kind of block.
			if (blocks[player.col][player.row + 1].shape == CORONA){
				outcome(CORONA);
				player.row++;
			}
			else if (blocks[player.col][player.row + 1].shape == BURGER){
				outcome(BURGER);
				player.row++;
			}
			else if (blocks[player.col][player.row + 1].shape == WELL){
				outcome(WELL);
				player.row++;
			}
			else if (blocks[player.col][player.row + 1].shape == FIRE){
				outcome(FIRE);
				player.row++;
			}
			else if (blocks[player.col][player.row + 1].shape == ROCK){
				outcome(ROCK);
			}
			else if (blocks[player.col][player.row + 1].shape == GOLD){
				player.row++;
				gameState = "Won";
			}
		}
		// Checks for if the player has less than 0 health
		if (player.life <= 0 || player.health <= 0)
		{
			gameState = "Lost";
		}
	}
	// provide implementation for rest of moves
}