#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Objective-C primarily uses dynamic dispatch for method calls due to its roots in the dynamic 
  runtime of Smalltalk. Dynamic dispatch means that the actual method to be executed is determined 
  at runtime based on the object’s class. This runtime decision-making allows for a high degree of 
  flexibility, such as the ability to change method implementations at runtime, support for 
  late binding, and extensive use of introspection.

  However, the flexibility of dynamic dispatch comes at the cost of performance. Each method 
  call goes through a process known as message sending, where the runtime looks up the method 
  implementation in the object’s dispatch table. This lookup introduces overhead for every method 
  call, which can lead to less efficient execution compared to static dispatch.

  A vtable (virtual table) is essentially a lookup table that stores pointers to the correct virtual 
  functions for each class.
*/


typedef struct Figure Figure;

typedef struct FigureVTable {
    /// Function pointer
    void (*draw)(Figure* self);
} FigureVTable;

struct Figure {
    FigureVTable vtable;
};


typedef struct Rect {
    Figure base;
    int perimeter;
} Rect;

typedef struct Circle {
    Figure base;
    int radius;
} Circle;


/* -- -- -- "Virtual" functions -- -- -- */

/// It is polymorphic in one parameter only: The special receiver parameter, called self or this, 
/// is the only parameter through which run-time polymorphism is typically possible. Polymorphism 
/// on other parameters can be simulated with helper methods in those types, which is awkward, 
/// and return-type polymorphism is impossible.

void draw_rect(Figure* self) {
    /// Cast back to get access to Rect-specific data (Downcast)
    Rect* rect = (Rect*)self;

    printf("Drawing Rect with perimeter: %d\n", rect->perimeter);
}

void draw_circle(Figure* self) {
    Circle* circle = (Circle*)self;
    printf("Drawing Circle with radius: %d\n", circle->radius);
}


/* -- -- -- Vtables -- -- -- */

static FigureVTable rect_vtable = {
    .draw = draw_rect
};

static FigureVTable circle_vtable = {
    .draw = draw_circle
};


/* -- -- -- Constructors -- -- -- */

Figure* rect_new(int perimeter) {
    Rect* rect = malloc(sizeof(Rect));
    rect->perimeter = perimeter;
    rect->base.vtable = rect_vtable;

    /// Type cast to Figure* pointer (Upcast)
    /// -> in fact it's rather relabeling the same pointer with different type information.
    /// It's possible since both Figure and Rect has same memory layout
    return (Figure*)rect;              
}

Figure* circle_new(int radius) {
    Circle* circle = malloc(sizeof(Circle));
    circle->radius = radius;
    circle->base.vtable = circle_vtable; 
    return (Figure*)circle;              
}


void call_stuff(Figure *fig) {
    fig->vtable.draw(fig);
}

int main(void) {
    Figure *rect = rect_new(10);
    call_stuff(rect);

    Figure *circle = circle_new(8);
    call_stuff(circle);

    return 0;
}
