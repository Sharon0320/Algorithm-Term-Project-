# Algorithm-Term-Project-


#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

// 2차원 좌표 구조체 정의
typedef struct {
    long long x, y;
} Point;

// 다각형을 나타내는 배열 구조체 정의
typedef struct {
    Point points[100]; // 다각형의 꼭짓점들을 저장하는 배열
    int size;          // 다각형의 꼭짓점 개수
} PointArray;

// 전역 변수 선언
int t, n, m;

// 벡터 외적 계산 함수 (Counter Clockwise)
long long ccw(long long x1, long long y1, long long x2, long long y2, long long x3, long long y3) {
    long long cw = (x1 * y2 + x2 * y3 + x3 * y1) - (y1 * x2 + y2 * x3 + y3 * x1);
    if (cw > 0) return 1;    // 반시계 방향
    if (cw < 0) return -1;   // 시계 방향
    return 0;                // 일직선
}

// 볼록 껍질 생성 함수
void createConvexHull(PointArray* pos, PointArray* hull) {
    // 기준점 정렬
    for (int i = 1; i < pos->size; i++) {
        if (pos->points[i].x < pos->points[0].x || (pos->points[i].x == pos->points[0].x && pos->points[i].y < pos->points[0].y)) {
            Point temp = pos->points[0];
            pos->points[0] = pos->points[i];
            pos->points[i] = temp;
        }
    }

    // 기준점으로부터 반시계 방향으로 정렬
    for (int i = 1; i < pos->size - 1; i++) {
        for (int j = i + 1; j < pos->size; j++) {
            if (ccw(pos->points[0].x, pos->points[0].y, pos->points[i].x, pos->points[i].y, pos->points[j].x, pos->points[j].y) < 0) {
                Point temp = pos->points[i];
                pos->points[i] = pos->points[j];
                pos->points[j] = temp;
            }
        }
    }

    // 볼록 껍질 생성
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

// 다각형의 꼭짓점 입력 함수
void inputPos(PointArray* pos, int size) {
    for (int i = 0; i < size; i++) {
        scanf("%lld %lld", &pos->points[i].x, &pos->points[i].y);
        pos->size++;
    }
}

// 점이 다각형 내부에 있는지 여부를 확인하는 함수
int isInside(PointArray* outer, Point* p) {
    if (outer->size <= 2) return 0;

    int cwBase = ccw(outer->points[0].x, outer->points[0].y, outer->points[1].x, outer->points[1].y, p->x, p->y);
    for (int i = 1; i < outer->size; i++) {
        int cw = ccw(outer->points[i].x, outer->points[i].y, outer->points[(i + 1) % outer->size].x, outer->points[(i + 1) % outer->size].y, p->x, p->y);
        if (cw != cwBase) return 0;
    }
    return 1;
}

// 선분이 교차하는지 확인하는 함수
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

// 두 다각형이 서로를 가로지르지 않고 분리되어 있는지 확인하는 함수
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
