/*
 * Copyright (C) 2013 Lucien XU <sfietkonstantin@free.fr>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * The names of its contributors may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#ifndef OSIGNAL_H
#define OSIGNAL_H

/**
 * @file osignal.h
 * @short Useful macros for Qt connection system
 */

/**
 * @short Helper macro for Qt connection system
 *
 * This macro generates casts an overloaded method
 * to a give signature.
 *
 * @param CLASS class name.
 * @param NAME method name.
 */
#define OSIGNAL0(CLASS, NAME) \
    static_cast<void (CLASS::*)()>(&CLASS::NAME)
/**
 * @short Helper macro for Qt connection system
 *
 * This macro generates casts an overloaded method
 * to a give signature.
 *
 * @param CLASS class name.
 * @param NAME method name.
 * @param ARG1 first argument type.
 */
#define OSIGNAL1(CLASS, NAME, ARG1) \
    static_cast<void (CLASS::*)(ARG1)>(&CLASS::NAME)
/**
 * @short Helper macro for Qt connection system
 *
 * This macro generates casts an overloaded method
 * to a give signature.
 *
 * @param CLASS class name.
 * @param NAME method name.
 * @param ARG1 first argument type.
 * @param ARG2 second argument type.
 */
#define OSIGNAL2(CLASS, NAME, ARG1, ARG2) \
    static_cast<void (CLASS::*)(ARG1, ARG2)>(&CLASS::NAME)

/**
 * @short Helper macro for Qt connection system
 *
 * This macro generates casts an overloaded method
 * to a give signature.
 *
 * @param CLASS class name.
 * @param NAME method name.
 */
#define OSLOT0(CLASS, NAME) \
    static_cast<void (CLASS::*)()>(&CLASS::NAME)
/**
 * @short Helper macro for Qt connection system
 *
 * This macro generates casts an overloaded method
 * to a give signature.
 *
 * @param CLASS class name.
 * @param NAME method name.
 * @param ARG1 first argument type.
 */
#define OSLOT1(CLASS, NAME, ARG1) \
    static_cast<void (CLASS::*)(ARG1)>(&CLASS::NAME)
/**
 * @short Helper macro for Qt connection system
 *
 * This macro generates casts an overloaded method
 * to a give signature.
 *
 * @param CLASS class name.
 * @param NAME method name.
 * @param ARG1 first argument type.
 * @param ARG2 second argument type.
 */
#define OSLOT2(CLASS, NAME, ARG1, ARG2) \
    static_cast<void (CLASS::*)(ARG1, ARG2)>(&CLASS::NAME)

#endif // OSIGNAL_H
