#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int x, y;
} Point;

// Global first point (p0) needed for comparing points
Point p0;

// Calculate square of distance between two points
int distSq(Point p1, Point p2) {
  return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

// Find orientation of triplet (p, q, r)
// Returns:
// 0 --> Collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r) {
  int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
  if (val == 0)
    return 0;               // collinear
  return (val > 0) ? 1 : 2; // clockwise or counterclockwise
}

// Function used by qsort() to sort points by polar angle
int compare(const void *vp1, const void *vp2) {
  Point *p1 = (Point *)vp1;
  Point *p2 = (Point *)vp2;

  int o = orientation(p0, *p1, *p2);
  if (o == 0)
    return (distSq(p0, *p2) >= distSq(p0, *p1)) ? -1 : 1;

  return (o == 2) ? -1 : 1;
}

// Swap two points
void swap(Point *p1, Point *p2) {
  Point temp = *p1;
  *p1 = *p2;
  *p2 = temp;
}

// Print the convex hull
void convexHull(Point points[], int n) {
  // Find the bottommost point
  int ymin = points[0].y, min = 0;
  for (int i = 1; i < n; i++) {
    int y = points[i].y;

    // Pick the bottom-most or choose the left most point in case of tie
    if ((y < ymin) || (ymin == y && points[i].x < points[min].x))
      ymin = points[i].y, min = i;
  }

  // Place the bottom-most point at first position
  swap(&points[0], &points[min]);

  // Sort n-1 points with respect to the first point
  p0 = points[0];
  qsort(&points[1], n - 1, sizeof(Point), compare);

  // Remove collinear points
  int m = 1;
  for (int i = 1; i < n; i++) {
    // Skip collinear points
    while (i < n - 1 && orientation(p0, points[i], points[i + 1]) == 0)
      i++;

    points[m] = points[i];
    m++;
  }

  // If modified array has less than 3 points, convex hull is not possible
  if (m < 3) {
    printf("Convex hull not possible!\n");
    return;
  }

  // Create the stack for convex hull points
  Point hull[m];
  int top = 2; // Initial stack size

  hull[0] = points[0];
  hull[1] = points[1];
  hull[2] = points[2];

  // Process remaining points
  for (int i = 3; i < m; i++) {
    // Keep removing top while the angle formed by points
    // hull[top-1], hull[top], and points[i] makes a non-left turn
    while (top > 0 && orientation(hull[top - 1], hull[top], points[i]) != 2)
      top--;

    top++;
    hull[top] = points[i];
  }

  // Print Result
  printf("\nPoints in the Convex Hull:\n");
  for (int i = 0; i <= top; i++)
    printf("(%d, %d)\n", hull[i].x, hull[i].y);
}

int main() {
  int n;
  printf("Enter the number of points: ");
  scanf("%d", &n);

  if (n < 3) {
    printf("Convex hull requires at least 3 points.\n");
    return 1;
  }

  Point *points = (Point *)malloc(n * sizeof(Point));
  if (!points) {
    printf("Memory allocation failed.\n");
    return 1;
  }

  printf("Enter the coordinates of the points (x y):\n");
  for (int i = 0; i < n; i++) {
    printf("Point %d: ", i + 1);
    scanf("%d %d", &points[i].x, &points[i].y);
  }

  convexHull(points, n);

  free(points);
  return 0;
} /*
 Enter the number of points: 7
 Enter the coordinates of the points (x y):
 Point 1: 0 0
 Point 2: 1 1
 Point 3: 2 2
 Point 4: 3 0
 Point 5: 2 -2
 Point 6: 0 -2
 Point 7: -1 0

 Points in the Convex Hull:
 (0, -2)
 (2, -2)
 (3, 0)
 (2, 2)
 (-1, 0)
 */