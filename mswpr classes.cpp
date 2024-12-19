#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

// Using an enum to track state.
// Enums rename integers, so "Up" will be replaced by '0' when the code compiles, and "Down" will be replaced with '1'
enum TileState {
    Up,
    Down
};

enum ButtonState {
    Left, Right, Middle, XButton1, XButton2, ButtonCount
};

struct Tile {
    // Store pointers to texture to avoid storing a copy (it can get very expensive very quickly!)
    const sf::Texture* up_texture;
    const sf::Texture* down_texture;
    const sf::Texture* mine_texture;
    const sf::Texture* one;
    const sf::Texture* two;
    const sf::Texture* three;
    const sf::Texture* four;
    const sf::Texture* five;
    const sf::Texture* six;
    const sf::Texture* seven;
    const sf::Texture* eight;
    sf::Sprite sprite;
    sf::Sprite front_sprite;
    TileState state;
    bool isMine;
    bool flagged;
    int nearbyMines;
    vector<Tile*> neighbors;
    const sf::Texture* flaggedTexture;
    sf::Sprite flaggedSprite;

public:
    // Pass textures by reference and store by pointer to avoid copying the textures.
    // Pass x and y to indicate where on the screen the tile should appear
    Tile(const sf::Texture& up_texture, const sf::Texture& down_texture, const sf::Texture& mine_texture, const sf::Texture& on,const sf::Texture& t,const sf::Texture& th,const sf::Texture& fo,const sf::Texture& fi,const sf::Texture& si,const sf::Texture& se,const sf::Texture& ei, int x, int y, sf::Texture& flag) {
        // Set the texture pointers
        this->up_texture = &up_texture;
        this->down_texture = &down_texture;
        this->mine_texture = &mine_texture;
        // Set sprite to 'up' state and texture
        this->sprite = sf::Sprite(up_texture);
        this->front_sprite = sf::Sprite(down_texture);
        this->state = Up;
        isMine = false;
        flagged = false;
        this->one = &on;
        this->two = &t;
        this->three = &th;
        this->four = &fo;
        this->five = &fi;
        this->six = &si;
        this->seven = &se;
        this->eight = &ei;
        // Set the sprite's position on the screen
        sprite.setPosition(x, y);
        front_sprite.setPosition(x, y);
        flaggedSprite.setPosition(x, y);
        this->flaggedTexture = &flag;
    }
    Tile() {
        // Set the texture pointers
        // this->up_texture = &up_texture;
        // this->down_texture = &down_texture;
        // this->mine_texture = &mine_texture;
        // // Set sprite to 'up' state and texture
        // this->sprite = sf::Sprite(up_texture);
        // this->state = Up;
        // isMine = false;
        // this->one = &on;
        // this->two = &t;
        // this->three = &th;
        // this->four = &fo;
        // this->five = &fi;
        // this->six = &si;
        // this->seven = &se;
        // this->eight = &ei;
        // // Set the sprite's position on the screen
        // sprite.setPosition(0, 0);
        // front_sprite.setPosition(0, 0);
    }
    // Getter for the sprite
    sf::Sprite& get_sprite() {
        return this->sprite;
    }
    sf::Sprite& get_front_sprite() {
        if (state == Down) {
            return front_sprite;
        }
        return sprite;
    }
    // Function which outlines the logic of a tile being clicked
    void click() {
        //cout << "Click function has been entered" << endl;
        // Check the current state and flip to the opposite state
         // if (state == Up) {
            sprite.setTexture(*down_texture);
             // state = Down;

            // cout << "TileState is up" << endl;
            // if (isMine) {
            //     front_sprite.setTexture(*mine_texture);
            // }
            // if(flagged) {
            //     flaggedSprite.setTexture(*flaggedTexture);
            // }
            // else {
            //     if(nearbyMines == 1) {
            //         front_sprite.setTexture(*one);
            //     }
            //     else if(nearbyMines == 2) {
            //         front_sprite.setTexture(*two);
            //     }
            //     else if(nearbyMines == 3) {
            //         front_sprite.setTexture(*three);
            //     }
            //     else if(nearbyMines == 4) {
            //         front_sprite.setTexture(*four);
            //     }
            //     else if(nearbyMines == 5) {
            //         front_sprite.setTexture(*five);
            //     }
            //     else if(nearbyMines == 6) {
            //         front_sprite.setTexture(*six);
            //     }
            //     else if(nearbyMines == 7) {
            //         front_sprite.setTexture(*seven);
            //     }
            //     else if(nearbyMines == 8) {
            //         front_sprite.setTexture(*eight);
            //     }
            //     else {
            //         front_sprite.setTexture(*up_texture);
            //     }
            // }
         // }
        // else { // State is down
        //     sprite.setTexture(*up_texture);
        //     state = Up;
        //     // if(flagged) {
        //     //     flaggedSprite.setTexture(*flaggedTexture);
        //     // }
        // }

    }

    void draw(sf::RenderWindow& w) {
        if (state == Up) {
            sprite.setTexture(*up_texture);
            w.draw(sprite);
            if(flagged) {
                flaggedSprite.setTexture(*flaggedTexture);
                w.draw(flaggedSprite);
            }
        }
        else {
            sprite.setTexture(*down_texture);
            w.draw(sprite);
            if(isMine) {
                front_sprite.setTexture(*mine_texture);
                w.draw(front_sprite);
            }
            else if(nearbyMines > 0) {
                if(nearbyMines == 1) {
                    front_sprite.setTexture(*one);
                }
                else if(nearbyMines == 2) {
                    front_sprite.setTexture(*two);
                }
                else if(nearbyMines == 3) {
                    front_sprite.setTexture(*three);
                }
                else if(nearbyMines == 4) {
                    front_sprite.setTexture(*four);
                }
                else if(nearbyMines == 5) {
                    front_sprite.setTexture(*five);
                }
                else if(nearbyMines == 6) {
                    front_sprite.setTexture(*six);
                }
                else if(nearbyMines == 7) {
                    front_sprite.setTexture(*seven);
                }
                else if(nearbyMines == 8) {
                    front_sprite.setTexture(*eight);
                }
                w.draw(front_sprite);
            }
        }
    }

    void revealTiles() {
        cout << "different" << endl;
        bool hasMines = false;
        for (int i = 0; i < neighbors.size(); i++) {
            if (neighbors.at(i)->isMine) {
                hasMines = true;
            }
        }

        if(state == Up && !(isMine) && !flagged) {
            state = Down;
            cout << "changed" << endl;
            for(int i = 0; i < neighbors.size(); i++) {
                // if(neighbors[i]->state == Down && neighbors[i]->isMine == false && neighbors[i]->flagged == false) {
                if(neighbors[i]->isMine == false && neighbors[i]->flagged == false) {
                    cout << i << endl;

                    if(!hasMines) {
                        neighbors[i]->revealTiles();
                    }
                    // neighbors[i]->revealTiles();
                    // neighbors[i]->sprite.setTexture(*down_texture);
                    // neighbors[i]->state = Down;


                }
            }
        }

        // if(!hasMines && state == Down) {
        //     for(int i = 0; i < neighbors.size(); i++) {
        //         neighbors[i]->state = Down;
        //     }
        // }
        //state = Down;
    }
};

