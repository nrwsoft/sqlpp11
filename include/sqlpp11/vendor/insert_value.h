/*
 * Copyright (c) 2013, Roland Bock
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 *   Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SQLPP_INSERT_VALUE_H
#define SQLPP_INSERT_VALUE_H

#include <sqlpp11/default_value.h>
#include <sqlpp11/null.h>
#include <sqlpp11/tvin.h>
#include <sqlpp11/type_traits.h>
#include <sqlpp11/vendor/interpreter.h>

namespace sqlpp
{
	namespace vendor
	{
		template<typename ValueType>
			struct insert_value_t
			{
				using _is_insert_value = std::true_type;
				using _value_t = ValueType;

				insert_value_t(_value_t value):
					_is_null(false),
					_is_default(false),
					_value({value})
				{}

				insert_value_t(tvin_t<_value_t> tvin):
					_is_null(tvin._is_trivial()),
					_is_default(false),
					_value({tvin._value})
				{}

				insert_value_t(const ::sqlpp::null_t&):
					_is_null(true),
					_is_default(false),
					_value()
				{}

				insert_value_t(const ::sqlpp::default_value_t&):
					_is_null(false),
					_is_default(true),
					_value()
				{}

				insert_value_t(const insert_value_t&) = default;
				insert_value_t(insert_value_t&&) = default;
				insert_value_t& operator=(const insert_value_t&) = default;
				insert_value_t& operator=(insert_value_t&&) = default;
				~insert_value_t() = default;

				bool _is_null;
				bool _is_default;
				typename wrap_operand<_value_t>::type _value;
			};

		template<typename Context, typename ValueType>
			struct interpreter_t<Context, insert_value_t<ValueType>>
			{
				using T = insert_value_t<ValueType>;

				static Context& _(const T& t, Context& context)
				{
					if (t._is_null)
						context << "NULL";
					else if (t._is_default)
						context << "DEFAULT";
					else
						interpret(t._value, context);
					return context;
				}
			};

	}
}

#endif
