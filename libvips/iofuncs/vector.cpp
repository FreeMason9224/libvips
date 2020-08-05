/* helper functions for Highway
 *
 * 29/07/21 kleisauke
 * 	- from vector.c
 */

/*

	This file is part of VIPS.

	VIPS is free software; you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
	02110-1301  USA

 */

/*

	These files are distributed with VIPS - http://www.vips.ecs.soton.ac.uk

 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /*HAVE_CONFIG_H*/
#include <glib/gi18n-lib.h>

#include <stdlib.h>

#include <vips/vips.h>
#include <vips/vector.h>
#include <vips/debug.h>
#include <vips/internal.h>

#ifdef HAVE_HWY
#include <hwy/highway.h>
#endif /*HAVE_HWY*/

/* Cleared by the command-line `--vips-novector` switch and the
 * `VIPS_NOVECTOR` env var.
 */
gboolean vips__vector_enabled = TRUE;

void
vips__vector_init(void)
{
	/* Check whether any features are being disabled by the environment.
	 */
	const char *env;
	if ((env = g_getenv("VIPS_VECTOR")))
		return vips_vector_disable_targets(
			g_ascii_strtoll(env, NULL, 0));

	/* Look for the deprecated IM_NOVECTOR environment variable as well.
	 */
	if (g_getenv("VIPS_NOVECTOR")
#if ENABLE_DEPRECATED
		|| g_getenv("IM_NOVECTOR")
#endif
	)
		vips__vector_enabled = FALSE;
}

gboolean
vips_vector_isenabled(void)
{
#ifdef HAVE_HWY
	return vips__vector_enabled && vips_vector_get_supported_targets() != 0;
#else
	return FALSE;
#endif
}

void
vips_vector_set_enabled(gboolean enabled)
{
	vips__vector_enabled = enabled;
}

/**
 * vips_vector_get_builtin_targets:
 *
 * Gets a bitfield of builtin targets that libvips was built with.
 *
 * Returns: a bitfield of builtin targets.
 */
gint64
vips_vector_get_builtin_targets(void)
{
#ifdef HAVE_HWY
	return HWY_TARGETS;
#else
	return 0;
#endif
}

/**
 * vips_vector_get_supported_targets:
 *
 * Gets a bitfield of enabled targets that are supported on this CPU. The
 * targets returned may change after calling vips_vector_disable_targets().
 *
 * Returns: a bitfield of supported CPU targets.
 */
gint64
vips_vector_get_supported_targets(void)
{
#ifdef HAVE_HWY
	return hwy::SupportedTargets() & ~(HWY_EMU128 | HWY_SCALAR);
#else
	return 0;
#endif
}

/**
 * vips_vector_target_name:
 * @target: A specific target to describe.
 *
 * Generates a human-readable ASCII string descriptor for a specific target.
 *
 * Returns: a string describing the target.
 */
const char *
vips_vector_target_name(gint64 target)
{
#ifdef HAVE_HWY
	return hwy::TargetName(target);
#else
	return NULL;
#endif
}

/**
 * vips_vector_disable_targets:
 * @disabled_targets: A bitfield of targets to disable at runtime.
 *
 * Takes a bitfield of targets to disable on the runtime platform.
 * Handy for testing and benchmarking purposes.
 *
 * This can also be set using the `VIPS_VECTOR` environment variable.
 */
void
vips_vector_disable_targets(gint64 disabled_targets)
{
#ifdef HAVE_HWY
	hwy::SetSupportedTargetsForTest(
		vips_vector_get_supported_targets() & ~disabled_targets);
#endif
}
