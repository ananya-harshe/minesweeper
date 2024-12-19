#include <iostream>
#include <SFML/Graphics.hpp>
#include "mswpr classes.cpp"
#include <fstream>
#include <cstdlib>
#include <map>
#include <chrono>
#include <SFML/Window/Mouse.hpp>
using namespace std;
using namespace sf;

map<int, Sprite> parseDigits(Sprite digits) {
  map<int, Sprite> digitsMap;
  for(int i = 0; i < 10; i++) {
    IntRect rect(i*21, 0, 21, 32);
    digits.setTextureRect(rect);
    Sprite sprite = digits;
    digitsMap.emplace(i, sprite);
  }
  return digitsMap;
}

//method from the documentation pdf
void setText(Text &text, float x, float y){
  FloatRect textRect = text.getLocalBounds();
  text.setOrigin(textRect.left + textRect.width/2.0f,
textRect.top + textRect.height/2.0f);
  text.setPosition(Vector2f(x, y));
}

// Function to convert a string to uppercase
string toUpperCase(const string& str) {
  string result = str;
  for (char& c : result) {
    c = toupper(c);
  }
  return result;
}

string capitalize(const string& str) {
  string result;
  if(!str.empty()) {
    result += toupper(str[0]);
    for(int i = 1; i<str.size(); i++) {
      result += tolower(str[i]);
    }
  }
  return result;
}

string username() {
  //code from the welcome window demo from class by daniel iirc?
  ifstream dimensions;
  dimensions.open("Project 3 - Minesweeper Spring 2024/files/config.cfg");
  string rowCount;
  string colCount;
  if (dimensions.is_open()) {
    getline(dimensions, colCount);
    getline(dimensions, rowCount);
  }
  // Create a window with the given dimensions and title
    int height = (stoi(rowCount) * 32) + 100;
    int width = stoi(colCount) * 32;
    sf::RenderWindow window(sf::VideoMode(width, height), "minesweeper enter name");

    // Load a font
    sf::Font font;
    if (!font.loadFromFile("Project 3 - Minesweeper Spring 2024/files/font.ttf")) {
      return "";
    }

    // Create text objects
    Text welcome("WELCOME TO MINESWEEPER!", font, 24);
    welcome.setStyle(Text::Bold | Text::Underlined);
    welcome.setFillColor(sf::Color::White);
    setText(welcome, width/2, (height/2) - 150);

    Text promptText("Enter your name:", font, 20);
    promptText.setStyle(Text::Bold);
    promptText.setFillColor(sf::Color::White);
    setText(promptText, width/2, (height/2) - 75);

    Text nameText("", font, 18);
    nameText.setStyle(Text::Bold);
    nameText.setFillColor(sf::Color::Yellow);
    setText(nameText, width/2, (height/2) - 45);

    string playerName;

    // Run the program as long as the window is open
    while (window.isOpen()) {
      // Check all the window's events that were triggered since the last iteration of the loop
      sf::Event event;
      while (window.pollEvent(event)) {
        // "Close requested" event: we close the window
        if (event.type == sf::Event::Closed) {
          window.close();
          return "";
        }

        // Handle text entered events
        if (event.type == sf::Event::TextEntered) {
          // Handle backspace key
          if (event.text.unicode == 8) { // 8 is the ASCII code for backspace
            if (!playerName.empty()) {
              playerName.pop_back();
            }
          }
          else if (event.text.unicode < 128) {
            char enteredChar = static_cast<char>(event.text.unicode);
            if(isalpha(enteredChar) && playerName.size() < 10) {
              playerName += enteredChar;
            }
          }
        }
      }

      // Convert the player's name to uppercase
      string capitalized_name = capitalize(playerName);
      nameText.setString(capitalized_name + "|");

      // Clear the window with blue color
      window.clear(sf::Color::Blue);

      // Draw the text objects
      window.draw(welcome);
      window.draw(promptText);
      window.draw(nameText);

      // End the current frame
      window.display();
      if(event.text.unicode == 58 && playerName.size() >= 1) {
        break;
      }
    }
  if(playerName.size() == 0) {return "";}
  return playerName;
}

int display_leaderboard(string player_time, string player_name) {
  ifstream dimensions;
  dimensions.open("Project 3 - Minesweeper Spring 2024/files/config.cfg");
  string rowCount;
  string colCount;
  if (dimensions.is_open()) {
    getline(dimensions, colCount);
    getline(dimensions, rowCount);
  }
  // Create a window with the given dimensions and title
    int height =  (stoi(rowCount) * 16) + 50;
    int width = stoi(colCount) * 16;
    sf::RenderWindow window(sf::VideoMode(width, height), "minesweeper leaderboard");

    // Load a font
    sf::Font font;
    if (!font.loadFromFile("Project 3 - Minesweeper Spring 2024/files/font.ttf")) {
      return -1;
    }

    // Create text objects
    Text heading("LEADERBOARD", font, 20);
    heading.setStyle(Text::Bold | Text::Underlined);
    heading.setFillColor(sf::Color::White);
    setText(heading, width/2, (height/2) - 150);

    map<string, string> leaderboard;

    ifstream winners;
    string ti;
    winners.open("Project 3 - Minesweeper Spring 2024/files/leaderboard.txt");
    int i = 0;
    while(i < 5) {
      string time;
      getline(winners, time, ',');
      string playerName;
      getline(winners, playerName);
      leaderboard[time] = playerName;
      i++;
    }
    leaderboard[player_time] = player_name + "*";
    // Run the program as long as the window is open
    while (window.isOpen()) {
      // Check all the window's events that were triggered since the last iteration of the loop
      sf::Event event;
      while (window.pollEvent(event)) {
        // "Close requested" event: we close the window
        if (event.type == sf::Event::Closed) {
          window.close();
          return 1;
        }

        // Handle text entered events

      }


      // Clear the window with blue color
      window.clear(sf::Color::Blue);


      window.draw(heading);
      map<string, string>::iterator it = leaderboard.begin();
      int i = 1;
      for(; it != leaderboard.end(); it++) {
        if(i <= 5) {
          Text rank(to_string(i) + ".", font, 18);
          rank.setStyle(Text::Bold);
          rank.setFillColor(sf::Color::White);
          setText(rank, width/2 - 100, (height/2) + (20*i));

          Text name(it->second, font, 18);
          name.setStyle(Text::Bold);
          name.setFillColor(sf::Color::White);
          setText(name, width/2 + 60, (height/2) + (20*i));

          Text time(it->first, font, 18);
          time.setStyle(Text::Bold);
          time.setFillColor(sf::Color::White);
          setText(time, width/2 - 20, (height/2) + (20*i));

          window.draw(rank);
          window.draw(time);
          window.draw(name);
          i++;
        }
        else {
          break;
        }
      }

      // End the current frame
      window.display();

    }
  return 0;
}

// index = i * num_cols + j

int nearbyMines(vector<vector<Tile>> tiles, int i, int j, int rowCount, int colCount) {
  int mineCount = 0;
  if((i + 1 < rowCount) && (j+1) < colCount && tiles[i+1][j+1].isMine) {
    mineCount += 1;
  }
  if((i < rowCount) && (j+1) < colCount && tiles[(i)][j+1].isMine) {
    mineCount += 1;
  }
  if((i - 1 >= 0) && (j+1) < colCount && tiles[(i-1)][j+1].isMine) {
    mineCount += 1;
  }
  if((i + 1 < rowCount) && (j < colCount) < colCount && tiles[(i+1)][j].isMine) {
    mineCount += 1;
  }
  if((i - 1 >= 0) && (j < colCount) < colCount && tiles[(i-1)][j].isMine) {
    mineCount += 1;
  }
  if((i + 1 < rowCount) && (j-1) >= 0 && tiles[(i+1)][j-1].isMine) {
    mineCount += 1;
  }
  if((i < rowCount) && (j-1) >= 0 && tiles[(i)][j-1].isMine) {
    mineCount += 1;
  }
  if((i - 1 >= 0) && (j-1) >= 0 && tiles[(i-1)][j-1].isMine) {
    mineCount += 1;
  }
  tiles[i][j].nearbyMines = mineCount;
  return mineCount;
}

// bool revealTiles(vector<vector<Tile>> neighbors) {
//   int i = 0;
//   int j = 0;
//   if(neighbors[i][j].front_sprite.getTexture()) {
//     return true;
//   }
//   else {
//     return neighbors[i+1][j+1].front_sprite.getTexture();
//   }
// }

bool won(vector<vector<Tile>>& tiles) {
  for(auto& row : tiles) {
    for(auto& tile : row) {
      if(tile.isMine && tile.state == Up) {
        return false;
      }
    }
  }
  return true;
}

int main() {
      string name = username();
      if(name != "") {
        //all time logic from the YouTube video demo by Alex Johnson and functionality for displaying the tiles from class demo by daniel iirc
        auto start_time = chrono::high_resolution_clock::now();
        auto pauseTime = chrono::high_resolution_clock::now();
        auto elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pauseTime).count();
        bool paused = false;

      sf::RenderWindow window;
        ifstream dimensions;
        dimensions.open("Project 3 - Minesweeper Spring 2024/files/config.cfg");
        string rowCount;
        string colCount;
        if (dimensions.is_open()) {
          getline(dimensions, colCount);
          getline(dimensions, rowCount);
        }

        int rc = stoi(rowCount); int cc = stoi(colCount);
        // Create a window with the given dimensions and title
        int purble_height = (rc * 32) + 100;
        int purble_width = cc * 32;
      window.create(sf::VideoMode(purble_width, purble_height), "mine the sweeper");

      // Load the texture files
      sf::Texture up_texture;
      sf::Texture down_texture;
      up_texture.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/tile_hidden.png");
      down_texture.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/tile_revealed.png");
        Texture faceHappyTexture, digitsTexture, pauseTexture, mineTexture, deadFaceTexture, debugTexture, leaderboardTexture, one, two, three, four, five, six, seven, eight, playTexture, flagTexture, won_face;
        faceHappyTexture.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/face_happy.png");
        digitsTexture.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/digits.png");
        pauseTexture.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/pause.png");
        mineTexture.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/mine.png");
        deadFaceTexture.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/face_lose.png");
        debugTexture.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/debug.png");
        leaderboardTexture.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/leaderboard.png");
        one.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/number_1.png");
        two.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/number_2.png");
        three.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/number_3.png");
        four.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/number_4.png");
        five.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/number_5.png");
        six.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/number_6.png");
        seven.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/number_7.png");
        eight.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/number_8.png");
        playTexture.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/play.png");
        flagTexture.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/flag.png");
        won_face.loadFromFile("Project 3 - Minesweeper Spring 2024/files/images/face_win.png");


        sf::Sprite faceHappy(faceHappyTexture);
        Sprite deadFace(deadFaceTexture);
        sf::Sprite digits(digitsTexture);
        sf::Sprite pause(pauseTexture);
        sf::Sprite mine(mineTexture);
        Sprite play(playTexture);
        Sprite leaderboard(leaderboardTexture);
        Sprite debug(debugTexture);
        Sprite flag(flagTexture);
        Sprite win(won_face);

        map<int, Sprite> digitsMap = parseDigits(digits);

        // Set positions for bottom images
        faceHappy.setPosition((stoi(colCount) / 2.0) * 32, 32 * (stoi(rowCount) + 0.5));  // Center position for the smiley face
        deadFace.setPosition((stoi(colCount) / 2.0) * 32, 32 * (stoi(rowCount) + 0.5));  // Center position for the dead face
        //digits.setPosition(10, 500);      // Position for the mine counter (left)
        win.setPosition((stoi(colCount) / 2.0) * 32, 32 * (stoi(rowCount) + 0.5));
        debug.setPosition((stoi(colCount) * 32) - 304, 32 * (stoi(rowCount) + 0.5));      // Position for the debug button (right)
        pause.setPosition((stoi(colCount) * 32) - 240, 32 * (stoi(rowCount) + 0.5));      // Position for the pause button (right)
        play.setPosition((stoi(colCount) * 32) - 240, 32 * (stoi(rowCount) + 0.5));
        leaderboard.setPosition((stoi(colCount) * 32) - 176, 32 * (stoi(rowCount) + 0.5));      // Position for the leaderboard button (right)




      // Create the tiles
      vector<vector<Tile>> tiles;
        tiles.resize(rc);
        for (int i = 0; i < rc; i++) {
          tiles[i].resize(cc);
        }
        for (int i = 0; i < rc; i++) {
          for (int j = 0; j < cc; j++) {
            tiles[i][j]=Tile(up_texture, down_texture, mineTexture,one, two, three, four, five, six, seven, eight, j*32, i*32, flagTexture);
          }
        }
      for (int x = 0; x < stoi(rowCount); x++) { // Iterating by 32 because the tile PNGs are 32 pixels wide
        for (int y = 0; y < stoi(colCount); y++) { // Iterating by 32 because the tile PNGs are 32 pixels tall
          vector<Tile*> neighbors;
          for(int nx = -1; nx <= 1; nx++) {
            for(int ny = -1; ny <= 1; ny++) {
              int x_coord = x + nx;
              int y_coord = y + ny;
              if(x_coord >= 0 && x_coord < rc && y_coord >= 0 && y_coord < cc && !(nx == 0 && ny == 0)) {
                neighbors.push_back(&tiles[x_coord][y_coord]);
              }
            }
          }


          // x and y correspond to where on the screen the top-left corner of the tile sprite should be
          tiles[x][y].neighbors = neighbors;
        }
      }

      //tiles[0].isMine = true;
      // set 50 random tiles as mines
        for(int i = 0; i < 50; i++) {
          int index = rand() % tiles.size();
          int indey = rand() % tiles[0].size();
          tiles[index][indey].isMine = true;
        }
        int mines = 50;

        for(int i = 0; i < rc; i++) {
          for(int j = 0; j < cc; j++) {
            int nm = nearbyMines(tiles, i, j, stoi(rowCount), stoi(colCount));

            if(nm == 1) {
              tiles[i][j].front_sprite.setTexture(one);
            }
            else if(nm == 2) {
              tiles[i][j].front_sprite.setTexture(two);
            }
            else if(nm == 3) {
              tiles[i][j].front_sprite.setTexture(three);
            }
            else if(nm == 4) {
              tiles[i][j].front_sprite.setTexture(four);
            }
            else if(nm == 5) {
              tiles[i][j].front_sprite.setTexture(five);
            }
            else if(nm == 6) {
              tiles[i][j].front_sprite.setTexture(six);
            }
            else if(nm == 7) {
              tiles[i][j].front_sprite.setTexture(seven);
            }
            else if(nm == 8) {
              tiles[i][j].front_sprite.setTexture(eight);
            }
          }
        }

        //when click on a tile, if it is a number, you reveal and stop
        //else if it is a blank, reveal it and search its neighbor tiles
        //repeat until all available tiles are open/reveal
      // Mainloop
      while (window.isOpen()) {
        bool jungwon = false;
        bool lost = false;
        // Event loop
        sf::Event event;
        while (window.pollEvent(event)) {
          // Close event
          if (event.type == sf::Event::EventType::Closed) {
            window.close();
            break;
          }
          // Click event
          if (event.type == sf::Event::EventType::MouseButtonPressed) {
            // Get click data
            auto click = event.mouseButton;
            // See if the click intersects any tiles. If so, run their click() function
            //int i = 0;
            if(Event::MouseButtonPressed == event.type && Mouse::Left == event.mouseButton.button) {
              for (int i = 0; i < stoi(rowCount); i++) {
                for (int j = 0; j < stoi(colCount); j++) {
                  // getGlobalBounds().contains(x, y) tests if (x, y) touches the sprite
                  if (tiles[i][j].get_sprite().getGlobalBounds().contains(click.x, click.y)) {
                    tiles[i][j].click(); // Click function as defined in the tile class
                    if(!paused && !jungwon) {
                      if(won(tiles)) {
                        jungwon = true;
                        paused = true;
                        auto timescore = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start_time);
                        int final_time = timescore.count() - elapsed_paused_time;
                        string time = to_string(final_time);
                        for(int i = 0; i < rc; i++) {
                          for(int j = 0; j < cc; j++) {
                            if(tiles[i][j].isMine) {
                              tiles[i][j].flagged = true;
                            }
                          }
                        }
                        int s = display_leaderboard(time, name);
                      }
                    }
                    if(tiles[i][j].isMine) {
                      paused = true;
                      lost = true;
                      window.draw(deadFace);
                      for(int x = 0; x < stoi(rowCount); x++) {
                        for(int y = 0; y < stoi(colCount); y++) {
                          tiles[x][y].state = Down;
                        }
                      }
                      //int leaderboard = display_leaderboard();
                    }
                    else {
                      tiles[i][j].revealTiles();
                      // cout << "changed" << endl;
                    }
                    //bool revealed = revealTiles(tiles, i);
                    //i++;
                  }
                }
              }
              if(pause.getGlobalBounds().contains(click.x, click.y) && !lost && !jungwon) {
                paused = !paused;
                if(paused) {
                  pauseTime = chrono::high_resolution_clock::now();
                }
                else {
                  auto unpausedTime = chrono::high_resolution_clock::now();
                  elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unpausedTime - pauseTime).count();
                }
              }


            }
            if(Event::MouseButtonPressed == event.type && Mouse::Right == event.mouseButton.button && !lost && !paused && !jungwon) {
              for(int x = 0; x < stoi(rowCount); x++) {
                for(int y = 0; y < stoi(colCount); y++) {
                  if(tiles[x][y].get_sprite().getGlobalBounds().contains(click.x, click.y)) {
                    tiles[x][y].flagged = !tiles[x][y].flagged;
                    if(tiles[x][y].isMine && tiles[x][y].flagged) {
                      mines--;
                    }
                    if(tiles[x][y].isMine && !tiles[x][y].flagged) {
                      mines++;
                    }
                    //tiles[x][y].flag();
                  }
                }
              }
            }

            if (debug.getGlobalBounds().contains(click.x, click.y)) {
              for (int x = 0; x < rc; x++) {
                for (int y = 0; y < cc; y++) {
                  if(tiles[x][y].isMine) {
                    if(tiles[x][y].state == Down) {
                      tiles[x][y].state = Up;
                    }
                    else {
                      tiles[x][y].state = Down;
                    }
                  }
                  // tiles[x][y].state = Down;
                  // tiles[x][y].draw(window);
                  // //bool revealed = revealTiles(tile.neighbors);
                  // if(tiles[x][y].isMine) {
                  //
                  // }
                }
              }
            }

            if(faceHappy.getGlobalBounds().contains(click.x, click.y)) {

            }
          }

          if (event.type == sf::Event::EventType::MouseButtonPressed) {
            int s;
            // Get click data
            auto click = event.mouseButton;
            // getGlobalBounds().contains(x, y) tests if (x, y) touches the sprite
            map<int, int> x_and_y;
            if (leaderboard.getGlobalBounds().contains(click.x, click.y)) {
              s = display_leaderboard("95:14", "");
              paused = true;
              for (int x = 0; x < stoi(rowCount); x++) {
                for (int y = 0; y < stoi(colCount); y++) {
                  if(tiles[x][y].state == Up) {
                    x_and_y.emplace(x, y);
                    tiles[x][y].state = Down;
                  }
                }
              }
            }
            map<int, int>::iterator it = x_and_y.begin();
            if(s == 1) {
              for (int x = 0; x < stoi(rowCount); x++) {
                for (int y = 0; y < stoi(colCount); y++) {
                  if (it == x_and_y.end()) {
                    break;
                  }
                  else if(x == it->first && y == it->second) {
                    tiles[x][y].click();
                  }
                  // if(it == x_and_y.end()) {
                  //   break;
                  // }
                  ++it;
                }
              }
            }
          }

        }
        // Render loop
        window.clear(sf::Color::White);

        auto game_duration = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start_time);
        int total_time = game_duration.count();
        int minutes;
        int seconds;

        if(paused) {
          total_time = total_time - elapsed_paused_time;
          minutes = total_time / 60;
          seconds = total_time % 60;
        }

        int minutes0 = minutes / 10 % 10;
        int minutes1 = minutes % 10;
        int seconds0 = seconds / 10 % 10;
        int seconds1 = seconds % 10;
        digitsMap[minutes0].setPosition((cc * 32) - 97,32 * (rc +0.5)+16);
        window.draw(digitsMap[minutes0]);
        digitsMap[minutes1].setPosition((cc * 32) - 97 + 21,32 * (rc +0.5)+16);
        window.draw(digitsMap[minutes1]);
        digitsMap[seconds0].setPosition((cc * 32) - 54 + 21,32 * (rc +0.5)+16);
        window.draw(digitsMap[seconds0]);
        digitsMap[seconds1].setPosition((cc * 32) - 54 + 63,32 * (rc +0.5)+16);
        window.draw(digitsMap[seconds1]);

        digitsMap[0].setPosition(33,32 * (rc +0.5) + 16);
        int tens = mines / 10;
        int ones = mines % 10;
        digitsMap[tens].setPosition(33 + 25,32 * (rc +0.5) + 16);
        window.draw(digitsMap[tens]);
        digitsMap[ones].setPosition(33 + 50,32 * (rc +0.5) + 16);
        window.draw(digitsMap[ones]);

        for (int x = 0; x < stoi(rowCount); x++) {
          for (int y = 0; y < stoi(colCount); y++) {
            tiles[x][y].draw(window);
            //cout << tiles[x][y].state << endl;
            //window.draw(tiles[x][y].get_sprite());
            //tiles[x][y].front_sprite.setTexture(down_texture);
            //window.draw(tiles[x][y].get_front_sprite());
          }
        }
        window.draw(faceHappy);
        window.draw(debug);
        window.draw(pause);
        window.draw(leaderboard);
        if(paused) {
          window.draw(play);
        }

        window.display();
      }
      }
  return 0;
}