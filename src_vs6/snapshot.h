#ifndef SNAP_H
#define SNAP_H

extern void Snapshot();
extern bool DrawVisuals;;

typedef void(__stdcall *glReadPixels_t)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid*);
extern void __stdcall m_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
extern glReadPixels_t glReadPixels_s;

#endif
