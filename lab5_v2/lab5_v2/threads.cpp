#include "threads.hpp"
#include "support.hpp"

#include <cstdlib>
#include <ctime>
#include <cassert>
#include <Tchar.h>

/*****************************************************************************/

void Invalidator( PVOID _pvoid ) {
	PINVALIDATORPAR pParams = (PINVALIDATORPAR)_pvoid;
	
	while ( ! pParams->m_bKill ) {
		InvalidateRect( pParams->m_hWnd, NULL, TRUE );

		Sleep( 20 );
	}
}

/*****************************************************************************/

void AlertThreadCreatureFail() {
	// FIXME: Тут должно быть какое-то более логичное действие
	assert( !"Failed to create thread" );
}

void AlertSemaphoreCreatureFail() {
	// FIXME: Тут должно быть какое-то более логичное действие
	assert( !"Failed to create semaphore" );
}

/*****************************************************************************/

void GrowSemaphore( PVOID _pvoid ) {
	PPARAMS pParams = (PPARAMS)_pvoid;
	HANDLE newSemaphore;

	// Генерируем новый семафор
	newSemaphore = CreateSemaphore( NULL, 0, pParams->m_maxNOfWorkers * 2, NULL );

	// Если создание семофора не удалось - ругаемся и выходим
	if ( newSemaphore == NULL ) {
		AlertSemaphoreCreatureFail();

		return;
	}

	EnterCriticalSection( &pParams->m_semBlocker );

	// Блокируем доступ к семофору
	ResetEvent( pParams->m_semAvailable );

	// Закрываем описатель старого семофора
	CloseHandle( pParams->m_semaphore );

	// Сохраняем новые параметры
	pParams->m_semaphore = newSemaphore;
	pParams->m_maxNOfWorkers *= 2;

	// Снимаем блокировку доступа к семофору
	SetEvent( pParams->m_semAvailable );

	LeaveCriticalSection( &pParams->m_semBlocker );
}

/*****************************************************************************/

enum class Direction {
	DONT_MOVE = -1, LEFT, RIGHT, UP, DOWN, N_OF_DIRECTIONS
};

inline Direction GetRandDirection() {
	return (Direction)( rand() % (int)Direction::N_OF_DIRECTIONS );
}

void DetermineDirection( Direction & _targetDir, const RECT & _figureRect, const RECT & _clientRect ) {
	if ( _figureRect.left < _clientRect.left )
		_targetDir = Direction::RIGHT;

	else if ( _figureRect.right > _clientRect.right )
		_targetDir = Direction::LEFT;

	else if ( _figureRect.top < _clientRect.top )
		_targetDir = Direction::DOWN;

	else if ( _figureRect.bottom > _clientRect.bottom )
		_targetDir = Direction::UP;

	//else {} // Оставить неизменной
}

void MoveInDirection( RECT & _targetFigure, const Direction _direction, const INT _step = 1 ) {
	switch ( _direction )
	{
	case Direction::LEFT:
		OffsetRect( &_targetFigure, -_step, 0 );
		break;

	case Direction::RIGHT:
		OffsetRect( &_targetFigure, +_step, 0 );
		break;

	case Direction::UP:
		OffsetRect( &_targetFigure, 0, -_step );
		break;

	case Direction::DOWN:
		OffsetRect( &_targetFigure, 0, +_step );

	default:
		break;
	}
}

void WalkCircleThread( PVOID _pvoid ) {
	//srand( time( NULL ) );

	PPARAMS pParams = (PPARAMS)_pvoid;

	const SIZE circleSize = { 40, 40 };

	HDC hdc;

	POINT startPoint;

	std::srand( std::time( NULL ) );

	Direction direction;

	direction = (Direction)( ( std::rand() ) % (int)Direction::N_OF_DIRECTIONS );

	startPoint = {
		std::rand() % ( pParams->m_clientRect.right  - circleSize.cx ),
		std::rand() % ( pParams->m_clientRect.bottom - circleSize.cy )
	};

	RECT circleDims = { 
		startPoint.x,
		startPoint.y,
		startPoint.x + circleSize.cx,
		startPoint.y + circleSize.cy
	}; // left top right bottom

	const UINT step = 1;

	// Проверяем на необходимость выхода из потока
	while ( ! pParams->m_bKill )
	{
		// Определяем направление движения окружности
		DetermineDirection( direction, circleDims, pParams->m_clientRect );

		// Определяем след. позицию окружности
		MoveInDirection( circleDims, direction, step );

		// Ждем доступа к семофору
		WaitForSingleObject( pParams->m_semAvailable, INFINITE );

		EnterCriticalSection( &pParams->m_semBlocker );

		// Получаем свою квоту на перерисовку
		WaitForSingleObject( pParams->m_semaphore, INFINITE );

		LeaveCriticalSection( &pParams->m_semBlocker );

		// Блокируем отрисовку в других потоках
		EnterCriticalSection( &pParams->m_drawBlocker );

		// Рисуем окружность
		Ellipse( pParams->m_hdc, circleDims );

		// Снимаем блокировку
		LeaveCriticalSection( &pParams->m_drawBlocker );

		// Ожидаем завершения отрисовки во всех остальных потоках
		EnterSynchronizationBarrier( &pParams->m_drawFinished,
			SYNCHRONIZATION_BARRIER_FLAGS_BLOCK_ONLY );
	}
}
