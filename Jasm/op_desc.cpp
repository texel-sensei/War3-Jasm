#include "op_desc.h"

op_sig signatures[] = {
	{}, // 0
	{OP_ENDPROGRAM	,{par_type::ignored	, par_type::ignored	, par_type::ignored	}, par_type::ignored	},
	{}, // 2
	{OP_FUNCTION	,{par_type::ignored	, par_type::ignored	, par_type::type	}, par_type::function	},
	{OP_ENDFUNCTION	,{par_type::ignored	, par_type::ignored	, par_type::ignored	}, par_type::ignored	},
	{OP_LOCAL		,{par_type::ignored	, par_type::ignored	, par_type::type	}, par_type::variable	},
	{OP_GLOBAL		,{par_type::ignored	, par_type::ignored	, par_type::type	}, par_type::variable	},
	{OP_CONSTANT	,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_POPFUNCARG	,{par_type::ignored	, par_type::integer	, par_type::type	}, par_type::variable	},
	{}, // 9
	{}, // A
	{OP_CLEANSTACK	,{par_type::ignored	, par_type::ignored	, par_type::integer	}, par_type::ignored	},
	{OP_LITERAL		,{par_type::ignored	, par_type::type	, par_type::reg		}, par_type::integer	},
	{OP_SETRET		,{par_type::ignored	, par_type::reg		, par_type::ignored	}, par_type::ignored	},
	{OP_GETVAR		,{par_type::ignored	, par_type::type	, par_type::reg		}, par_type::variable	},
	{OP_CODE		,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_GETARRAY	,{par_type::type	, par_type::reg		, par_type::reg		}, par_type::variable	},
	{OP_SETVAR		,{par_type::ignored	, par_type::ignored	, par_type::reg		}, par_type::variable	},
	{OP_SETARRAY	,{par_type::ignored	, par_type::reg		, par_type::reg		}, par_type::variable	},
	{OP_PUSH		,{par_type::ignored	, par_type::ignored	, par_type::reg		}, par_type::ignored	},
	{OP_SETRIGHT	,{par_type::ignored	, par_type::ignored	, par_type::reg		}, par_type::ignored	},
	{OP_NATIVE		,{par_type::ignored	, par_type::ignored	, par_type::ignored }, par_type::function	},
	{OP_JASSCALL	,{par_type::ignored	, par_type::ignored	, par_type::ignored }, par_type::function	},
	{OP_I2R			,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_AND			,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_OR			,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_EQUAL		,{par_type::reg		, par_type::reg		, par_type::reg		}, par_type::unknown	},
	{OP_NOTEQUAL	,{par_type::reg		, par_type::reg		, par_type::reg		}, par_type::unknown	},
	{OP_LESSEREQUAL ,{par_type::reg		, par_type::reg		, par_type::reg		}, par_type::unknown	},
	{OP_GREATEREQUAL,{par_type::reg		, par_type::reg		, par_type::reg		}, par_type::unknown	},
	{OP_LESSER		,{par_type::reg		, par_type::reg		, par_type::reg		}, par_type::unknown	},
	{OP_GREATER		,{par_type::reg		, par_type::reg		, par_type::reg		}, par_type::unknown	},
	{OP_ADD			,{par_type::reg		, par_type::reg		, par_type::reg		}, par_type::ignored	},
	{OP_SUB			,{par_type::reg		, par_type::reg		, par_type::reg		}, par_type::ignored	},
	{OP_MUL			,{par_type::reg		, par_type::reg		, par_type::reg		}, par_type::ignored	},
	{OP_DIV			,{par_type::reg		, par_type::reg		, par_type::reg		}, par_type::ignored	},
	{OP_MODULO		,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_NEGATE		,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_NOT			,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_RETURN		,{par_type::ignored	, par_type::ignored	, par_type::ignored	}, par_type::ignored	},
	{OP_LABEL		,{par_type::ignored	, par_type::ignored	, par_type::ignored }, par_type::jmp_dest	},
	{OP_JUMPIFTRUE	,{par_type::ignored	, par_type::ignored	, par_type::reg		}, par_type::jmp_dest	},
	{OP_JUMPIFFALSE ,{par_type::ignored	, par_type::ignored	, par_type::reg		}, par_type::jmp_dest	},
	{OP_JUMP		,{par_type::ignored	, par_type::ignored	, par_type::ignored	}, par_type::jmp_dest	}
};	 