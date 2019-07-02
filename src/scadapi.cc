#include "scadapi.h"
#include "builtin.h"
#include "keyword.h"

ScadApi::ScadApi(QsciScintilla *qsci, QsciLexer *lexer) : QsciAbstractAPIs(lexer), qsci(qsci)
{
	for (auto iter = Builtins::temporaryNow.cbegin(); iter != Builtins::temporaryNow.cend(); ++iter)
	{
		QStringList calltipList;
		for(auto it = iter->second.cbegin(); it != iter->second.cend(); ++it)
			calltipList.append(QString::fromStdString(*it));

		funcs.append(ApiFunc(QString::fromStdString(iter->first), calltipList));
	}

	for(auto keyword: Builtins::keywordList)
	{
		QStringList calltipList;
		for(auto calltip: keyword->calltip)
			calltipList.append(QString::fromStdString(calltip));

		funcs.append(ApiFunc(QString::fromStdString(keyword->word), calltipList));
	}
}

ScadApi::~ScadApi()
{
}

void ScadApi::updateAutoCompletionList(const QStringList &context, QStringList &list)
{
	const QString c = context.last();
	for (int a = 0;a < funcs.size();a++) {
		const ApiFunc &func = funcs.at(a);
		const QString &name = func.get_name();
		if (name.startsWith(c)) {
			if (!list.contains(name)) {
				list << name;
			}
		}
	}
}

void ScadApi::autoCompletionSelected (const QString &selection)
{
}

QStringList ScadApi::callTips (const QStringList &context, int commas, QsciScintilla::CallTipsStyle style, QList< int > &shifts)
{
	QStringList callTips;
	for (int a = 0;a < funcs.size();a++) {
		if (funcs.at(a).get_name() == context.at(context.size() - 2)) {
			callTips = funcs.at(a).get_params();
			break;
		}
	}
	return callTips;
}