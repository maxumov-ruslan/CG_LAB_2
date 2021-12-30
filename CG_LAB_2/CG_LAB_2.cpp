// cgfx_lab2.cpp : Defines the entry point for the application.
//


#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include<vector>
using namespace std;

vector <sf::Vector2f > points_2 ={ 
{203.f, 212.f},
{224.f, 212.f},
{244.f, 224.f},
{275.f, 242.f},
{301.f, 254.f},
{335.f, 260.f},
{355.f, 252.f},
{370.f, 257.f},
{388.f, 251.f},
{407.f, 244.f},
{424.f, 238.f},
{451.f, 242.f},
{477.f, 247.f},
{500.f, 240.f},
{512.f, 233.f},
{529.f, 230.f},
{553.f, 226.f},
{568.f, 216.f},
{587.f, 195.f},
{607.f, 172.f},
{627.f, 156.f},
{645.f, 156.f},
{656.f, 169.f},
{658.f, 186.f},
{663.f, 205.f},
{664.f, 224.f},
{661.f, 241.f},
{657.f, 259.f},
{659.f, 282.f},
{666.f, 308.f},
{670.f, 329.f},
{675.f, 349.f},
{683.f, 372.f},
{692.f, 396.f},
{705.f, 421.f},
{717.f, 449.f},
{736.f, 468.f},
{766.f, 489.f},
{785.f, 496.f},
{791.f, 502.f},
{797.f, 515.f},
{798.f, 599.f},
{245.f, 598.f},
{234.f, 588.f},
{228.f, 571.f},
{220.f, 552.f},
{214.f, 534.f},
{212.f, 509.f},
{210.f, 490.f},
{212.f, 464.f},
{212.f, 445.f},
{215.f, 429.f},
{220.f, 413.f},
{214.f, 380.f},
{214.f, 350.f},
{205.f, 327.f},
{197.f, 306.f},
{188.f, 286.f},
{183.f, 264.f},
{184.f, 237.f},
{197.f, 213.f},
{203.f, 210.f}, };




sf::Vector2f operator* (const sf::Vector2f& a, double t) {
	return sf::Vector2f(a.x * t, a.y * t);
}
sf::Vector2f operator* (double t, const sf::Vector2f& a) {
	return sf::Vector2f(a.x * t, a.y * t);
}
sf::Vector2f operator+ (const sf::Vector2f& a, const sf::Vector2f& b) {
	return sf::Vector2f(a.x + b.x, a.y + b.y);
}
sf::Vector2f operator- (const sf::Vector2f& a, const sf::Vector2f& b) {
	return a + (-1.0) * b;
}
sf::Vector2f operator/ (const sf::Vector2f& a, double t) {
	return 1.0 / t * a;
}
class BezierCurve {
	std::vector<sf::Vector2f> points;
public:
	BezierCurve(std::vector<sf::Vector2f> points) {
		this->points.assign(points.begin(), points.end());
	}
	sf::Vector2f point(double t) {
		t = 1.0 - t;
		std::vector<sf::Vector2f> pts_old(points);
		std::vector<sf::Vector2f> pts_new;
		while (pts_old.size() > 1) {
			pts_new.resize(pts_old.size() - 1);
			for (int i = 0; i < pts_old.size() - 1; i++) {
				pts_new[i] = pts_old[i] * t + pts_old[i + 1] * (1 - t);
			}
			std::swap(pts_old, pts_new);
		}
		if (!pts_old.size())
			return sf::Vector2f{ 0,0 };
		return pts_old[0];

	}
};
class BCurve3 {
	std::vector<sf::Vector2f> vec;
	BezierCurve* curve;
public:
	BCurve3(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d) {
		vec.clear();
		vec.push_back(a);
		vec.push_back(b);
		vec.push_back(c);
		vec.push_back(d);
		curve = new BezierCurve(vec);
	}
	void toVertexArray(sf::VertexArray& va) {
		auto from = curve->point(0.0);
		auto to = curve->point(1.0);
		std::cout << "(" << from.x << ',' << from.y << ") -- (" << to.x << ',' << to.y << ")" << std::endl;
		for (double t = 0.0; t <= 1.0; t += 0.01) {
			va.append(sf::Vertex{ curve->point(t),sf::Color::White });

		}

	}
	static std::vector<BCurve3*> getCurvesForPoints(const std::vector<sf::Vector2f>& points) {
		std::vector<BCurve3*> curves;
		for (auto it = points.begin(); it != points.end() && it + 1 != points.end(); it++) {
			auto a = *it;
			auto b = *(it + 1);
			sf::Vector2f p0, p3;
			if (it == points.begin()) {
				p0 = a;
			}
			else {
				p0 = *(it - 1);
			}
			if (it + 2 == points.end()) {
				p3 = b;
			}
			else {
				p3 = *(it + 2);
			}
			std::cout << "a: " << a.x << ',' << a.y << std::endl;
			std::cout << "b: " << b.x << ',' << b.y << std::endl;
			curves.push_back(new BCurve3(a, a + (b - p0) / 6, b - (p3 - a) / 6, b));
		}


		return curves;

	}
	static void drawCurves(const std::vector<sf::Vector2f>& points, sf::VertexArray& va) {
		for (auto bc : getCurvesForPoints(points)) {
			bc->toVertexArray(va);
			delete bc;
		}

	}
	~BCurve3() {
		delete curve;
	}
};
int main()
{

	sf::VertexArray vertexarray(sf::LineStrip);
	//int n;
	//std::cin >> n;
	//std::cout << n << std::endl;
	std::vector<sf::Vector2f> points;
	//for (int i = 0; i < n; i++) {
	//	float x, y;
	//	std::cin >> x;
	//	std::cin >> y;
	//	std::cout << x << ' ' << y << std::endl;
	//	points.push_back(sf::Vector2f{ x,y });//сюда
	//}

	for (int i = 0; i < points_2.size(); i++)
	{
		if (i % 4 ==0)
		points.push_back(points_2[i]);//сюда
	}
	


	//points=points_2;//сюда
	
	sf::RenderWindow win(sf::VideoMode(800, 600), " lab2");
	BCurve3::drawCurves(points, vertexarray);

	while (win.isOpen()) {
		sf::Event event;
		while (win.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				win.close();
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				points.push_back(sf::Vector2f{ (float)event.mouseButton.x,(float)event.mouseButton.y });
				vertexarray.clear();
				BCurve3::drawCurves(points, vertexarray);
			}
		}
		win.clear();
		win.draw(vertexarray);
		for (int i = 0; i < points.size(); i++) {
			sf::Vector2f pos = points[i];
			sf::CircleShape node;
			node.setPosition(pos - sf::Vector2f{ 2,2 });
			node.setRadius(2.0);
			node.setFillColor(sf::Color::White);
			win.draw(node);
		}

		win.display();


	}
	return 0;
}
/*
62
203 212
224 212
244 224
275 242
301 254
335 260
355 252
370 257
388 251
407 244
424 238
451 242
477 247
500 240
512 233
529 230
553 226
568 216
587 195
607 172
627 156
645 156
656 169
658 186
663 205
664 224
661 241
657 259
659 282
666 308
670 329
675 349
683 372
692 396
705 421
717 449
736 468
766 489
785 496
791 502
797 515
798 599
245 598
234 588
228 571
220 552
214 534
212 509
210 490
212 464
212 445
215 429
220 413
214 380
214 350
205 327
197 306
188 286
183 264
184 237
197 213
203 210


*/