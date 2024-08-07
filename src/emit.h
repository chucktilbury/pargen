/**
 * @file emit.h
 *
 * @brief This is the public interface for the emitter. This is the only part
 * that appears in the main application. It does not implement a AST pass but
 * uses AST passes that implement an emitter.
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 2024-08-07
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _EMIT_H_
#define _EMIT_H_

void emit(void);

#endif /* _EMIT_H_ */
