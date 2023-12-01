#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

// 2���� ��ǥ ����ü ����
typedef struct {
    long long x, y;
} Point;

// �ٰ����� ��Ÿ���� �迭 ����ü ����
typedef struct {
    Point points[100]; // �ٰ����� ���������� �����ϴ� �迭
    int size;          // �ٰ����� ������ ����
} PointArray;

// ���� ���� ����
int t, n, m;

// ���� ���� ��� �Լ� (Counter Clockwise)
long long ccw(long long x1, long long y1, long long x2, long long y2, long long x3, long long y3) {
    long long cw = (x1 * y2 + x2 * y3 + x3 * y1) - (y1 * x2 + y2 * x3 + y3 * x1);
    if (cw > 0) return 1;    // �ݽð� ����
    if (cw < 0) return -1;   // �ð� ����
    return 0;                // ������
}

// ���� ���� ���� �Լ�
void createConvexHull(PointArray* pos, PointArray* hull) {
    // ������ ����
    for (int i = 1; i < pos->size; i++) {
        if (pos->points[i].x < pos->points[0].x || (pos->points[i].x == pos->points[0].x && pos->points[i].y < pos->points[0].y)) {
            Point temp = pos->points[0];
            pos->points[0] = pos->points[i];
            pos->points[i] = temp;
        }
    }

    // ���������κ��� �ݽð� �������� ����
    for (int i = 1; i < pos->size - 1; i++) {
        for (int j = i + 1; j < pos->size; j++) {
            if (ccw(pos->points[0].x, pos->points[0].y, pos->points[i].x, pos->points[i].y, pos->points[j].x, pos->points[j].y) < 0) {
                Point temp = pos->points[i];
                pos->points[i] = pos->points[j];
                pos->points[j] = temp;
            }
        }
    }

    // ���� ���� ����
    hull->size = 0;
    hull->points[hull->size++] = pos->points[0];
    hull->points[hull->size++] = pos->points[1];

    for (int i = 2; i < pos->size; i++) {
        while (hull->size >= 2 && ccw(hull->points[hull->size - 2].x, hull->points[hull->size - 2].y,
            hull->points[hull->size - 1].x, hull->points[hull->size - 1].y, pos->points[i].x, pos->points[i].y) <= 0) {
            hull->size--;
        }
        hull->points[hull->size++] = pos->points[i];
    }
}

// �ٰ����� ������ �Է� �Լ�
void inputPos(PointArray* pos, int size) {
    for (int i = 0; i < size; i++) {
        scanf("%lld %lld", &pos->points[i].x, &pos->points[i].y);
        pos->size++;
    }
}

// ���� �ٰ��� ���ο� �ִ��� ���θ� Ȯ���ϴ� �Լ�
int isInside(PointArray* outer, Point* p) {
    if (outer->size <= 2) return 0;

    int cwBase = ccw(outer->points[0].x, outer->points[0].y, outer->points[1].x, outer->points[1].y, p->x, p->y);
    for (int i = 1; i < outer->size; i++) {
        int cw = ccw(outer->points[i].x, outer->points[i].y, outer->points[(i + 1) % outer->size].x, outer->points[(i + 1) % outer->size].y, p->x, p->y);
        if (cw != cwBase) return 0;
    }
    return 1;
}

// ������ �����ϴ��� Ȯ���ϴ� �Լ�
int isCross(Point a, Point b, Point c, Point d) {
    int ab = ccw(a.x, a.y, b.x, b.y, c.x, c.y) * ccw(a.x, a.y, b.x, b.y, d.x, d.y);
    int cd = ccw(c.x, c.y, d.x, d.y, a.x, a.y) * ccw(c.x, c.y, d.x, d.y, b.x, b.y);

    if (ab == 0 && cd == 0) {
        if (a.x > b.x) {
            Point temp = a;
            a = b;
            b = temp;
        }
        if (c.x > d.x) {
            Point temp = c;
            c = d;
            d = temp;
        }
        return (c.x <= b.x && a.x <= d.x && c.y <= b.y && a.y <= d.y);
    }
    return (ab <= 0 && cd <= 0);
}

// �� �ٰ����� ���θ� ���������� �ʰ� �и��Ǿ� �ִ��� Ȯ���ϴ� �Լ�
int isDivide(PointArray* blackHull, PointArray* whiteHull) {
    if (isInside(blackHull, &whiteHull->points[0]) || isInside(whiteHull, &blackHull->points[0])) return 0;

    for (int i = 0; i < blackHull->size; i++) {
        if (isInside(whiteHull, &blackHull->points[i])) return 0;
    }

    for (int i = 0; i < whiteHull->size; i++) {
        if (isInside(blackHull, &whiteHull->points[i])) return 0;
    }

    for (int i = 0; i < blackHull->size; i++) {
        for (int j = 0; j < whiteHull->size; j++) {
            if (isCross(blackHull->points[i], blackHull->points[(i + 1) % blackHull->size],
                whiteHull->points[j], whiteHull->points[(j + 1) % whiteHull->size])) {
                return 0;
            }
        }
    }

    return 1;
}

int main() {
    scanf("%d", &t);
    while (t--) {
        PointArray blackPos = { .size = 0 };
        PointArray whitePos = { .size = 0 };
        PointArray blackHull = { .size = 0 };
        PointArray whiteHull = { .size = 0 };

        scanf("%d %d", &n, &m);

        inputPos(&blackPos, n);
        inputPos(&whitePos, m);

        createConvexHull(&blackPos, &blackHull);
        createConvexHull(&whitePos, &whiteHull);

        if (isDivide(&blackHull, &whiteHull)) {
            printf("YES\n");
        }
        else {
            printf("NO\n");
        }
    }
    return 0;
}
