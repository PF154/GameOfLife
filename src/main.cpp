// Run Conway's Game of Life in real time based on initial random noise

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <memory>

#include <SFML/Graphics.hpp>

const char block = 'O';

// return the index in a flattened array of the input 2d coords
inline int idx(int x, int y, int width)
{
	return y * width + x;
}

// Originially was rendering this to the terminal
// void moveCursorUp(int n) {
//     std::cout << "\033[" << n << "A";
// }

// void clearLine() {
//     std::cout << "\r\033[K";
// }

void computeTimeStep(int* in_data, int* out_data, int width, int height)
{
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			// Periodic boundaries
			int left_index = (x - 1 > 0) ? x - 1 : width - 1;
			int right_index = (x + 1 < width - 1) ? x + 1 : 0;
			int up_index = (y - 1 > 0) ? y - 1 : height - 1;
			int down_index = (y + 1 < height - 1) ? y + 1 : 0;
			
			int neighbors = 	in_data[idx(left_index, up_index, width)] +
								in_data[idx(x, up_index, width)] +
								in_data[idx(right_index, up_index, width)] +
								in_data[idx(left_index, y, width)] +
								in_data[idx(right_index, y, width)] +
								in_data[idx(left_index, down_index, width)] +
								in_data[idx(x, down_index, width)] +
								in_data[idx(right_index, down_index, width)];

			int cell_index = idx(x, y, width);
			if (in_data[cell_index] == 1) 
			{
				if (neighbors == 2 || neighbors == 3) 
				{
					out_data[cell_index] = 1;
				}
				else
				{
					out_data[cell_index] = 0;
				}
			}
			else if(in_data[cell_index] == 0)
			{
				if (neighbors == 3) 
				{
					out_data[cell_index] = 1;
				}
				else
				{
					out_data[cell_index] = 0;
				}
			}
		}
	}
}

void renderData(int* data, int width, int height)
{
	// moveCursorUp(height);
	for (int i = 0; i < height; i++)
	{
		// clearLine();
		for (int j = 0; j < width; j++)
		{
			if (data[idx(j, i, width)] == 1)
			{
				std::cout << block;
			}
			else
			{
				std::cout << ' ';
			}
		}
		std::cout << '\n';
	}
}

void color_map(int* in_data, std::uint8_t* color_out, int width, int height)
{
	for (int i = 0; i < width * height; i++)
	{
		color_out[i * 4] = 255 * in_data[i];
		color_out[i * 4 + 1] = 255 * in_data[i];
		color_out[i * 4 + 2] = 255 * in_data[i];
		color_out[i * 4 + 3] = 255;
	}
}

int main()
{
	const int Nx(200), Ny(200);

	std::unique_ptr<int[]> data_ping = std::make_unique<int[]>(Nx*Ny);
	std::unique_ptr<int[]> data_pong = std::make_unique<int[]>(Nx*Ny);

	std::unique_ptr<std::uint8_t[]> pixel_data = std::make_unique<std::uint8_t[]>(Nx*Ny*4);

	// Initialize data
	for (int i=0; i < Nx*Ny; i++) 
	{	
		data_ping[i] = (rand() % 2) % 2;
	}

	// const int timesteps = 1000;
	// for (int t = 0; t < timesteps; t++)
	sf::RenderWindow window(sf::VideoMode(sf::Vector2u{800, 800}), "Conway's Game of Life");

	sf::Texture texture(sf::Vector2u{Nx, Ny});
    sf::Sprite sprite(texture);
	sprite.setTexture(texture);

	sf::Vector2u windowSize = window.getSize();
	sf::Vector2u textureSize = texture.getSize();

	float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
	float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

	sprite.setScale(sf::Vector2(scaleX, scaleY));

	while (window.isOpen())
	{
		while (auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		computeTimeStep(data_ping.get(), data_pong.get(), Nx, Ny);

		// renderData(data_ping.get(), Nx, Ny);
		color_map(data_pong.get(), pixel_data.get(), Nx, Ny);


		texture.update(pixel_data.get());

		window.clear();
		window.draw(sprite);
		window.display();


		// Always go from ping to pong

		std::swap(data_ping, data_pong);

		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	}

}
