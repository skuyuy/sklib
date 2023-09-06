#pragma once

#define SK_DECL_TAG(name) struct tag_ ## name {}
#define SK_TAG(name) tag_ ## name