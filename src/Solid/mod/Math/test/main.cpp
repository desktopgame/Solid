#include <Math.hpp>
#include <cassert>
#include <iostream>

int main(int argc, char* argv[])
{
    using namespace Lib::Math;
    Vector2 v2;
    assert(Mathf::equals(v2.x(), 0.0f) && Mathf::equals(v2.y(), 0.0f));

    Vector2 v2a({ 1.0f, 1.0f });
    v2 += v2a;
    assert(Mathf::equals(v2.x(), 1.0f) && Mathf::equals(v2.y(), 1.0f));

    v2 *= 3.0f;
    assert(Mathf::equals(v2.x(), 3.0f) && Mathf::equals(v2.y(), 3.0f));

    Matrix matrix({
        Vector4({ 1.0f, 0.0f, 0.0f, 0.0f }),
        Vector4({ 0.0f, 1.0f, 0.0f, 0.0f }),
        Vector4({ 0.0f, 0.0f, 1.0f, 0.0f }),
        Vector4({ 0.0f, 0.0f, 0.0f, 1.0f }),
    });

    assert(Mathf::equals(matrix.row(0).at(0), 1.0f));
    assert(Mathf::equals(matrix.row(1).at(1), 1.0f));
    assert(Mathf::equals(matrix.row(2).at(2), 1.0f));
    assert(Mathf::equals(matrix.row(3).at(3), 1.0f));

    Matrix matrix2({
        Vector4({ 0.0f, 1.0f, 0.0f, 0.0f }),
        Vector4({ 0.0f, 0.0f, 0.0f, 0.0f }),
        Vector4({ 0.0f, 0.0f, 0.0f, 0.0f }),
        Vector4({ 0.0f, 0.0f, 0.0f, 0.0f }),
    });

    assert(Mathf::equals(matrix2.at(0, 1), 1.0f));

    Matrix matrix3 = Matrix::translate(Vector3({ 1, 2, 3 }));
    assert(Matrix::multiply(Matrix::inverse(matrix3), matrix3) == Matrix());

    Matrix matrix4 = Matrix::rotateX(45.0f);
    assert(Matrix::multiply(Matrix::inverse(matrix4), matrix4) == Matrix());

    {
        Matrix t = Matrix::translate(Vector3({ 1, 2, 3 }));
        assert(Matrix::multiply(Matrix::inverse(t), t) == Matrix());

        Matrix rx = Matrix::rotateX(Mathf::Deg2Rad * 45.0f);
        assert(Matrix::multiply(Matrix::inverse(rx), rx) == Matrix());

        Matrix ry = Matrix::rotateY(Mathf::Deg2Rad * 45.0f);
        assert(Matrix::multiply(Matrix::inverse(ry), ry) == Matrix());

        Matrix rz = Matrix::rotateZ(Mathf::Deg2Rad * 45.0f);
        assert(Matrix::multiply(Matrix::inverse(rz), rz) == Matrix());

        Matrix s = Matrix::scale(Vector3({ 1, 2, 3 }));
        assert(Matrix::multiply(Matrix::inverse(s), s) == Matrix());
    }

    {
        Vector3 point({ 1, 2, 3 });
        Vector3 translated = Matrix::multiply(Matrix::translate(Vector3({ 3, 1, 3 })), point);
        assert(translated == Vector3({ 4, 3, 6 }));

        Vector3 scaled = Matrix::multiply(Matrix::scale(Vector3({ 2, 2, 3 })), point);
        assert(scaled == Vector3({ 2, 4, 9 }));
    }
    return 0;
}