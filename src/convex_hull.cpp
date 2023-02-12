#include <iostream>
#include <string>
#include <numeric>
#include <cassert>
#include <optional>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

struct fraction
{
    long numerator   = 0;
    long denominator = 1;
};

struct point
{
    fraction x;
    fraction y;
};

struct line_segment
{
    point p1;
    point p2;
};

fraction simplify_fraction(fraction f)
{
    assert(f.denominator != 0);
    if (f.numerator < 0 && f.denominator < 0)
    {
        f.numerator   *= (-1);
        f.denominator *= (-1);
    }

    // std::gcd returns 0 if both numerator and denominator are 0
    int d = std::gcd(f.numerator, f.denominator);
    if (d > 0)
        return {f.numerator / d, f.denominator / d};

    return f;
}

fraction operator+(fraction lhs, fraction rhs)
{
    assert(lhs.denominator != 0 && rhs.denominator != 0);
    if (lhs.denominator == rhs.denominator)
        return {lhs.numerator + rhs.numerator, lhs.denominator};

    return {lhs.numerator * rhs.denominator + rhs.numerator * lhs.denominator,
            lhs.denominator * rhs.denominator};
}

fraction operator*(fraction lhs, fraction rhs)
{
    assert(lhs.denominator != 0 && rhs.denominator != 0);
    return {lhs.numerator * rhs.numerator, lhs.denominator * rhs.denominator};
}

fraction operator-(fraction lhs, fraction rhs)
{
    assert(lhs.denominator != 0 && rhs.denominator != 0);
    if (lhs.denominator == rhs.denominator)
        return {lhs.numerator - rhs.numerator, lhs.denominator};

    return {lhs.numerator * rhs.denominator - rhs.numerator * lhs.denominator,
            lhs.denominator * rhs.denominator};
}

fraction operator/(fraction lhs, fraction rhs)
{
    assert(lhs.denominator != 0 && rhs.denominator != 0);
    return {lhs.numerator * rhs.denominator, rhs.numerator * lhs.denominator};
}

string fraction_to_str(fraction f)
{
    assert(f.denominator != 0);
    if (f.denominator > 1)
    {
        return to_string(f.numerator) + "/" + to_string(f.denominator);
    }
    else
    {
        return to_string(f.numerator);
    }
}

string point_to_str(point& p)
{
    return "(" + fraction_to_str(p.x) + "," + fraction_to_str(p.y) + ")";
}

// Not sure if inputs can be assumed to be integers, just in case
// make code to handle fractions. Downside is this generates quite a bit
// of code.
point parse_xy_coord(string const& str)
{
    bool left;

    fraction a {};
    fraction b {};

    bool denomi = false;
    bool a_is_negative = false;
    bool b_is_negative = false;

    string a_numeri_str {};
    string a_denomi_str {};

    string b_numeri_str {};
    string b_denomi_str {};

    for (auto c : str)
    {
        if (c == '(') // begin parsing
        {
            left = true;
            continue;
        }

        if (c == ',') // switch sides
        {
            denomi = false;
            left = false;
            continue;
        }

        if (c == ')') // finish parsing
        {
            // a
            a.numerator = stoi(a_numeri_str);
            if (!a_denomi_str.empty())
                a.denominator = stoi(a_denomi_str);

            // b
            b.numerator = stoi(b_numeri_str);
            if (!b_denomi_str.empty())
                b.denominator = stoi(b_denomi_str);
            
            if (a_is_negative)
                a.numerator *= -1;

            if (b_is_negative)
                b.numerator *= -1;
            
            return {a, b};
        }

        if (left) // left side
        {
            if (c == '/')
            {
                denomi = true;
                continue;
            }

            if (c == '-')
            {
                a_is_negative =  true;
                continue;
            }

            if (denomi)
            {
                a_denomi_str += c;
            }
            else
            {
                a_numeri_str += c;
            }
        }
        else // right side
        {
            if (c == '/')
            {
                denomi = true;
                continue;
            }

            if (c == '-')
            {
                b_is_negative = true;
                continue;
            }

            if (denomi)
            {
                b_denomi_str += c;
            }
            else
            {
                b_numeri_str += c;
            }
        }
    }
    return {}; // when input is ill-formed
}

// return index of the leftmost point in points
int find_leftmost_point(std::vector<point> const& points)
{
    // Assume all the points are all integers even though the code for the fraction
    // stuff is recycled from the previous exercise
    int idx = 0;
    long x_val = LONG_MAX;
    for (size_t i = 0; i < points.size(); i++)
    {
        if (points[i].x.numerator < x_val)
        {
            idx = i;
            x_val = points[i].x.numerator;
        }
    }
    return idx;
}

int orientation(point a, point b, point c)
{
    fraction v = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);

    // colinear
    if (v.numerator == 0)
        return 0;

    if (v.numerator > 0)
    {
        return 1; // clockwise
    }
    else
    {
        return 2; // counterclockwise
    }
}

// Gift wrapping/Jarvis march
vector<point> convex_hull(vector<point> const& points)
{
    // There must be at least 3 points to form a valid hull
    if (points.size() < 3)
        return{};

    vector<point> hull;
    auto leftmost_idx = find_leftmost_point(points);

    int a = leftmost_idx;
    int b;

    while (true)
    {
        hull.push_back(points[a]);
        b = (a + 1) % points.size();

        // find most counterclockwise point, update its idx to b
        for (size_t i = 0; i < points.size(); i++)
        {
            if (orientation(points[a], points[i], points[b]) == 2)
                b = i;
        }
        a = b; // update a

        if (a == leftmost_idx)
            break;

    }
    return hull;
}

string ConvexHullPoints(string strArr[], int arrLength) 
{
    vector<point> points(arrLength);
    for (size_t i = 0; i < arrLength; i++)
    {
        points[i] = parse_xy_coord(strArr[i]);
    }

    auto hull = convex_hull(points);

  return to_string(hull.size());
}

int main()
{
    string arr[] = {"(2,2)", "(3,1)", "(2,6)", "(0,-2)"};
    auto res = ConvexHullPoints(arr, sizeof(arr) / sizeof(*arr));
    cout << res << "\n";
}