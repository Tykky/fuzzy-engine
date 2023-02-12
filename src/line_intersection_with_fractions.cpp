#include <iostream>
#include <string>
#include <numeric>
#include <cassert>
#include <optional>
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

std::optional<point> find_intersection(line_segment seg_a, line_segment seg_b)
{
    fraction x1 = seg_a.p1.x;
    fraction x2 = seg_a.p2.x;
    fraction y1 = seg_a.p1.y;
    fraction y2 = seg_a.p2.y;

    fraction x3 = seg_b.p1.x;
    fraction x4 = seg_b.p2.x;
    fraction y3 = seg_b.p1.y;
    fraction y4 = seg_b.p2.y;

    // Compute intersection points, simplifying fractions here helps avoiding integer overflow later down the line
    fraction px_numerator   = simplify_fraction({((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4))});
    fraction px_denominator = simplify_fraction({((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))});
    
    fraction py_numerator   = simplify_fraction({((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4))});
    fraction py_denominator = simplify_fraction({((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))});

    // When no solution is found
    if (px_denominator.numerator == 0 || py_denominator.numerator == 0)
        return {};

    fraction px = px_numerator / px_denominator;
    fraction py = py_numerator / py_denominator;

    return {{simplify_fraction(px), simplify_fraction(py)}};
}

string IntersectingLines(string strArr[], int arrLength) 
{
    assert(arrLength == 4);

    line_segment l1 = {parse_xy_coord(strArr[0]), parse_xy_coord(strArr[1])};
    line_segment l2 = {parse_xy_coord(strArr[2]), parse_xy_coord(strArr[3])};

    auto intersect = find_intersection(l1, l2);

    if (intersect.has_value())
        return point_to_str(intersect.value());
    
    return "no intersection";
}

int main()
{
    string input[] = {"(9/1,-2/1)","(-2/1,9/1)","(3/1,4/1)","(10/1,11/1)"};
    string binput[] = {"(1,2)", "(3,4)", "(-5, -6)", "(-7, -8)"};
    string cinput[] = {"(5, 2)", "(1, 10/7)", "(-5/2, -9/18)", "(1, 15/9)"};
    auto res = IntersectingLines(cinput, sizeof(cinput) / sizeof(*cinput));
    cout << res << "\n";
}