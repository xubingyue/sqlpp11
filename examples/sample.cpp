/*
 * Copyright (c) 2014, Roland Bock
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Sample.h"
#include "MockDb.h"
#include <sqlpp11/sqlpp11.h>

MockDb db;

test::TabPerson p;
test::TabFeature f;

int main()
{
	db(insert_into(f).set(f.name = "Loves C++", p.fatal = false));

	db(insert_into(f).set(p.name = "Roland", p.feature = 1));

	auto s = select(all_of(p))
				      .from(p, q)
						  .where(p.name == any(select(q.name)
										               .from(q)
																	 .where(true)))
							.group_by(q.name)
							.having(p.name.like("%Bee%"))
							.order_by(p.name.asc())
							.limit(3).offset(7);

	auto x = s.as(sqlpp::alias::x);
	for (const auto& row : db(select(p.id, x.name)
				.from(p.join(x).on(p.feature == x.feature))
				.where(true)))
	{
		int id = row.id;
		std::string name = row.name;
	}
}
