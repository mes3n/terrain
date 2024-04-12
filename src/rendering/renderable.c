#include "renderable.h"

#include <stdlib.h>

int
addRenderable(Renderable** head, const GLuint shader, const GLuint vao,
    const int size)
{
    if (*head == NULL)
    {
        *head = (Renderable*)malloc(sizeof(Renderable));
        if (*head == NULL)
        {
            return 0;
        }
        (*head)->shader = shader;
        (*head)->vao = vao;
        (*head)->size = size;
        (*head)->next = NULL;
        return 1;
    }
    return addRenderable(&(*head)->next, shader, vao, size);
}

void
removeRenderable(Renderable** head, const GLuint vao)
{
    if (*head == NULL)
    {
        return;
    }
    if ((*head)->vao == vao)
    {
        Renderable* next = (*head)->next;
        free(*head);
        *head = next;
    }
    return removeRenderable(&(*head)->next, vao);
}

void
clearRenderable(Renderable* head)
{
    if (head == NULL)
    {
        return;
    }
    clearRenderable(head->next);
    free(head);
}
